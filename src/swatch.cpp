/**
 * \file
 *
 * \author Mattia Basaglia
 *
 * \copyright Copyright (C) 2015 Mattia Basaglia
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "swatch.hpp"

#include "color_utils.hpp"

#include <QApplication>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QKeyEvent>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QToolTip>

#include <cmath>
#include <wobjectimpl.h>

W_OBJECT_IMPL(color_widgets::Swatch)

namespace color_widgets
{

class Swatch::Private
{
public:
  ColorPalette palette; ///< Palette with colors and related metadata
  int selected;         ///< Current selection index (-1 for no selection)
  QSize color_size;     ///< Preferred size for the color squares
  ColorSizePolicy size_policy;
  QPen border;
  QPen selection;
  int margin;
  QColor emptyColor;
  int forced_rows;
  int forced_columns;
  bool readonly; ///< Whether the palette can be modified via user interaction

  QPoint drag_pos;     ///< Point used to keep track of dragging
  int drag_index;      ///< Index used by drags
  int drop_index;      ///< Index for a requested drop
  QColor drop_color;   ///< Dropped color
  bool drop_overwrite; ///< Whether the drop will overwrite an existing color

  Swatch* owner;

  Private(Swatch* owner)
      : selected(-1)
      , color_size(16, 16)
      , size_policy(Hint)
      , border(Qt::black, 1)
      , selection(Qt::gray, 2, Qt::DotLine)
      , margin(0)
      , emptyColor(QColor(0, 0, 0, 0))
      , forced_rows(0)
      , forced_columns(0)
      , readonly(false)
      , drag_index(-1)
      , drop_index(-1)
      , drop_overwrite(false)
      , owner(owner)
  {
  }

  /**
   * \brief Number of rows/columns in the palette
   */
  QSize rowcols()
  {
    int count = palette.count();
    if (count == 0)
      return QSize();

    if (forced_rows)
      return QSize(std::ceil(float(count) / forced_rows), forced_rows);

    int columns = palette.columns();

    if (forced_columns)
      columns = forced_columns;
    else if (columns == 0)
      columns = qMin(palette.count(), owner->width() / color_size.width());

    int rows = std::ceil(float(count) / columns);

    return QSize(columns, rows);
  }

  /**
   * \brief Sets the drop properties
   */
  void dropEvent(QDropEvent* event)
  {
    // Find the output location
    drop_index = owner->indexAt(event->pos());
    if (drop_index == -1)
      drop_index = palette.count();

    // Gather up the color
    if (event->mimeData()->hasColor())
    {
      drop_color = event->mimeData()->colorData().value<QColor>();
    }
    else if (event->mimeData()->hasText())
    {
      drop_color = QColor(event->mimeData()->text());
    }

    drop_overwrite = false;
    QRectF drop_rect = indexRect(drop_index);
    if (drop_index < palette.count() && drop_rect.isValid())
    {
      // 1 column => vertical style
      if (palette.columns() == 1 || forced_columns == 1)
      {
        // Dragged to the last quarter of the size of the square, add after
        if (event->posF().y() >= drop_rect.top() + drop_rect.height() * 3.0 / 4)
          drop_index++;
        // Dragged to the middle of the square, overwrite existing color
        else if (
            event->posF().x() > drop_rect.top() + drop_rect.height() / 4
            && (event->dropAction() != Qt::MoveAction || event->source() != owner
                || palette.colorAt(drop_index) == emptyColor))
          drop_overwrite = true;
      }
      else
      {
        // Dragged to the last quarter of the size of the square, add after
        if (event->posF().x() >= drop_rect.left() + drop_rect.width() * 3.0 / 4)
          drop_index++;
        // Dragged to the middle of the square, overwrite existing color
        else if (
            event->posF().x() > drop_rect.left() + drop_rect.width() / 4
            && (event->dropAction() != Qt::MoveAction || event->source() != owner
                || palette.colorAt(drop_index) == emptyColor))
          drop_overwrite = true;
      }
    }

    owner->update();
  }

  /**
   * \brief Clears drop properties
   */
  void clearDrop()
  {
    drop_index = -1;
    drop_color = QColor();
    drop_overwrite = false;

    owner->update();
  }

  /**
   * \brief Actual size of a color square
   */
  QSizeF actualColorSize()
  {
    QSize rowcols = this->rowcols();
    if (!rowcols.isValid())
      return QSizeF();
    return actualColorSize(rowcols);
  }

  /**
   * \brief Actual size of a color square
   * \pre rowcols.isValid() and obtained via rowcols()
   */
  QSizeF actualColorSize(const QSize& rowcols)
  {
    return QSizeF(
        float(owner->width()) / rowcols.width(), float(owner->height()) / rowcols.height());
  }

  /**
   * \brief Rectangle corresponding to the color at the given index
   * \pre rowcols.isValid() and obtained via rowcols()
   * \pre color_size obtained via rowlcols(rowcols)
   */
  QRectF indexRect(int index, const QSize& rowcols, const QSizeF& color_size)
  {
    if (index == -1)
      return QRectF();

    return QRectF(
        index % rowcols.width() * color_size.width() + margin,
        index / rowcols.width() * color_size.height() + margin,
        color_size.width() - margin * 2,
        color_size.height() - margin * 2);
  }
  /**
   * \brief Rectangle corresponding to the color at the given index
   */
  QRectF indexRect(int index)
  {
    QSize rc = rowcols();
    if (index == -1 || !rc.isValid())
      return QRectF();
    return indexRect(index, rc, actualColorSize(rc));
  }
};

Swatch::Swatch(QWidget* parent) : QWidget(parent), p(new Private(this))
{
  connect(&p->palette, &ColorPalette::colorsChanged, this, &Swatch::paletteModified);
  connect(
      &p->palette, &ColorPalette::columnsChanged, this, (void (QWidget::*)()) & QWidget::update);
  connect(
      &p->palette, &ColorPalette::colorsUpdated, this, (void (QWidget::*)()) & QWidget::update);
  connect(&p->palette, &ColorPalette::colorChanged, [this](int index) {
    if (index == p->selected)
      colorSelected(p->palette.colorAt(index));
  });
  connect(&p->palette, &ColorPalette::colorRemoved, [this](int index) {
    if (index == p->selected)
      clearSelection();
  });
  setFocusPolicy(Qt::StrongFocus);
  setAcceptDrops(true);
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  setAttribute(Qt::WA_Hover, true);
}

Swatch::~Swatch()
{
  delete p;
}

QSize Swatch::sizeHint() const
{
  QSize rowcols = p->rowcols();

  if (!p->color_size.isValid() || !rowcols.isValid())
    return QSize();

  return QSize(
      (p->color_size.width() + p->margin * 2) * rowcols.width(),
      (p->color_size.height() + p->margin * 2) * rowcols.height());
}

QSize Swatch::minimumSizeHint() const
{
  if (p->size_policy != Hint)
    return sizeHint();
  return QSize();
}

const ColorPalette& Swatch::palette() const
{
  return p->palette;
}

ColorPalette& Swatch::palette()
{
  return p->palette;
}

int Swatch::selected() const
{
  return p->selected;
}

QColor Swatch::selectedColor() const
{
  return p->palette.colorAt(p->selected);
}

int Swatch::indexAt(const QPoint& pt)
{
  QSize rowcols = p->rowcols();
  if (rowcols.isEmpty())
    return -1;

  QSizeF color_size = p->actualColorSize(rowcols);

  QPoint point(
      qBound<int>(0, pt.x() / color_size.width(), rowcols.width() - 1),
      qBound<int>(0, pt.y() / color_size.height(), rowcols.height() - 1));

  int index = point.y() * rowcols.width() + point.x();
  if (index >= p->palette.count())
    return -1;
  return index;
}

QColor Swatch::colorAt(const QPoint& pt)
{
  return p->palette.colorAt(indexAt(pt));
}

void Swatch::setPalette(const ColorPalette& palette)
{
  clearSelection();
  p->palette = palette;
  update();
  paletteChanged(p->palette);
}

void Swatch::setSelected(int selected)
{
  if (selected < 0 || selected >= p->palette.count())
    selected = -1;

  if (selected != p->selected)
  {
    selectedChanged(p->selected = selected);
    if (selected != -1)
      colorSelected(p->palette.colorAt(p->selected));
    update();
  }
}

void Swatch::clearSelection()
{
  setSelected(-1);
}

void Swatch::paintEvent(QPaintEvent* event)
{
  QSize rowcols = p->rowcols();
  if (rowcols.isEmpty())
    return;

  QSizeF color_size = p->actualColorSize(rowcols);
  QPixmap alpha_pattern(detail::alpha_pixmap());
  QPen penEmptyBorder = p->border;
  QColor colorEmptyBorder = p->border.color();
  colorEmptyBorder.setAlpha(56);
  penEmptyBorder.setColor(colorEmptyBorder);
  QPainter painter(this);

  QStyleOptionFrame panel;
  panel.initFrom(this);
  panel.lineWidth = 1;
  panel.midLineWidth = 0;
  panel.state |= QStyle::State_Sunken;
  style()->drawPrimitive(QStyle::PE_Frame, &panel, &painter, this);
  QRect r = style()->subElementRect(QStyle::SE_FrameContents, &panel, this);
  painter.setClipRect(r);

  int count = p->palette.count();
  painter.setPen(p->border);
  for (int y = 0, i = 0; i < count; y++)
  {
    for (int x = 0; x < rowcols.width() && i < count; x++, i++)
    {
      if (p->palette.colorAt(i) == p->emptyColor)
      {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(penEmptyBorder);
        painter.drawRect(p->indexRect(i, rowcols, color_size));
        continue;
      }
      painter.setBrush(alpha_pattern);
      painter.drawRect(p->indexRect(i, rowcols, color_size));
      painter.setBrush(p->palette.colorAt(i));
      painter.drawRect(p->indexRect(i, rowcols, color_size));
    }
  }

  painter.setClipping(false);

  if (p->drop_index != -1)
  {
    QRectF drop_area = p->indexRect(p->drop_index, rowcols, color_size);
    if (p->drop_overwrite)
    {
      painter.setBrush(p->drop_color);
      painter.setPen(QPen(Qt::gray));
      painter.drawRect(drop_area);
    }
    else if (rowcols.width() == 1)
    {
      // 1 column => vertical style
      painter.setPen(QPen(p->drop_color, 2));
      painter.setBrush(Qt::transparent);
      painter.drawLine(drop_area.topLeft(), drop_area.topRight());
    }
    else
    {
      painter.setPen(QPen(p->drop_color, 2));
      painter.setBrush(Qt::transparent);
      painter.drawLine(drop_area.topLeft(), drop_area.bottomLeft());
      // Draw also on the previous line when the first item of a line is
      // selected
      if (p->drop_index % rowcols.width() == 0 && p->drop_index != 0)
      {
        drop_area = p->indexRect(p->drop_index - 1, rowcols, color_size);
        drop_area.translate(color_size.width(), 0);
        painter.drawLine(drop_area.topLeft(), drop_area.bottomLeft());
      }
    }
  }

  if (p->selected != -1)
  {
    QRectF rect = p->indexRect(p->selected, rowcols, color_size);
    painter.setBrush(Qt::transparent);
    painter.setPen(QPen(Qt::darkGray, 2));
    painter.drawRect(rect);
    painter.setPen(p->selection);
    painter.drawRect(rect);
  }
}

void Swatch::keyPressEvent(QKeyEvent* event)
{
  if (p->palette.count() == 0)
    QWidget::keyPressEvent(event);

  int selected = p->selected;
  int count = p->palette.count();
  QSize rowcols = p->rowcols();
  int columns = rowcols.width();
  int rows = rowcols.height();
  switch (event->key())
  {
    default:
      QWidget::keyPressEvent(event);
      return;

    case Qt::Key_Left:
      if (selected == -1)
        selected = count - 1;
      else if (selected > 0)
        selected--;
      break;

    case Qt::Key_Right:
      if (selected == -1)
        selected = 0;
      else if (selected < count - 1)
        selected++;
      break;

    case Qt::Key_Up:
      if (selected == -1)
        selected = count - 1;
      else if (selected >= columns)
        selected -= columns;
      break;

    case Qt::Key_Down:
      if (selected == -1)
        selected = 0;
      else if (selected < count - columns)
        selected += columns;
      break;

    case Qt::Key_Home:
      if (event->modifiers() & Qt::ControlModifier)
        selected = 0;
      else
        selected -= selected % columns;
      break;

    case Qt::Key_End:
      if (event->modifiers() & Qt::ControlModifier)
        selected = count - 1;
      else
        selected += columns - (selected % columns) - 1;
      break;

    case Qt::Key_Delete:
      removeSelected();
      return;

    case Qt::Key_Backspace:
      if (selected != -1 && !p->readonly)
      {
        p->palette.eraseColor(selected);
        if (p->palette.count() == 0)
          selected = -1;
        else
          selected = qMax(selected - 1, 0);
      }
      break;

    case Qt::Key_PageUp:
      if (selected == -1)
        selected = 0;
      else
        selected = selected % columns;
      break;
    case Qt::Key_PageDown:
      if (selected == -1)
      {
        selected = count - 1;
      }
      else
      {
        selected = columns * (rows - 1) + selected % columns;
        if (selected >= count)
          selected -= columns;
      }
      break;
  }
  setSelected(selected);
}

void Swatch::removeSelected()
{
  if (p->selected != -1 && !p->readonly)
  {
    int selected = p->selected;
    p->palette.eraseColor(p->selected);
    setSelected(qMin(selected, p->palette.count() - 1));
  }
}

void Swatch::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton)
  {
    setSelected(indexAt(event->pos()));
  }
  else if (event->button() == Qt::RightButton)
  {
    int index = indexAt(event->pos());
    if (index != -1)
      rightClicked(index);
  }
}

void Swatch::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton)
  {
    p->drag_index = -1;
  }
}

void Swatch::paletteModified()
{
  if (p->selected >= p->palette.count())
    clearSelection();

  if (p->size_policy == Minimum)
    setMinimumSize(sizeHint());
  else if (p->size_policy == Fixed)
    setFixedSize(sizeHint());

  update();
}

QSize Swatch::colorSize() const
{
  return p->color_size;
}

void Swatch::setColorSize(const QSize& colorSize)
{
  if (p->color_size != colorSize)
    colorSizeChanged(p->color_size = colorSize);
}

Swatch::ColorSizePolicy Swatch::colorSizePolicy() const
{
  return p->size_policy;
}

void Swatch::setColorSizePolicy(ColorSizePolicy colorSizePolicy)
{
  if (p->size_policy != colorSizePolicy)
  {
    setMinimumSize(0, 0);
    setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    paletteModified();
    colorSizePolicyChanged(p->size_policy = colorSizePolicy);
  }
}

int Swatch::forcedColumns() const
{
  return p->forced_columns;
}

int Swatch::forcedRows() const
{
  return p->forced_rows;
}

void Swatch::setForcedColumns(int forcedColumns)
{
  if (forcedColumns <= 0)
    forcedColumns = 0;

  if (forcedColumns != p->forced_columns)
  {
    forcedColumnsChanged(p->forced_columns = forcedColumns);
    forcedRowsChanged(p->forced_rows = 0);
  }
}

void Swatch::setForcedRows(int forcedRows)
{
  if (forcedRows <= 0)
    forcedRows = 0;

  if (forcedRows != p->forced_rows)
  {
    forcedColumnsChanged(p->forced_columns = 0);
    forcedRowsChanged(p->forced_rows = forcedRows);
  }
}

bool Swatch::readOnly() const
{
  return p->readonly;
}

void Swatch::setReadOnly(bool readOnly)
{
  if (readOnly != p->readonly)
  {
    readOnlyChanged(p->readonly = readOnly);
    setAcceptDrops(!p->readonly);
  }
}

bool Swatch::event(QEvent* event)
{
  if (event->type() == QEvent::ToolTip)
  {
    QHelpEvent* help_ev = static_cast<QHelpEvent*>(event);
    int index = indexAt(help_ev->pos());
    if (index != -1)
    {
      QColor color = p->palette.colorAt(index);
      QString name = p->palette.nameAt(index);
      QString message = color.name();
      if (!name.isEmpty())
        message = tr("%1 (%2)").arg(name).arg(message);
      message = "<tt style='background-color:" + color.name() + ";color:" + color.name()
                + ";'>MM</tt> " + message.toHtmlEscaped();
      QToolTip::showText(help_ev->globalPos(), message, this, p->indexRect(index).toRect());
      event->accept();
    }
    else
    {
      QToolTip::hideText();
      event->ignore();
    }
    return true;
  }

  return QWidget::event(event);
}

QPen Swatch::border() const
{
  return p->border;
}

void Swatch::setBorder(const QPen& border)
{
  if (border != p->border)
  {
    p->border = border;
    borderChanged(border);
    update();
  }
}

int Swatch::margin() const
{
  return p->margin;
}

void Swatch::setMargin(const int& margin)
{
  if (margin != p->margin)
  {
    p->margin = margin;
    marginChanged(margin);
    update();
  }
}

QPen Swatch::selection() const
{
  return p->selection;
}

void Swatch::setSelection(const QPen& selection)
{
  if (selection != p->selection)
  {
    p->selection = selection;
    selectionChanged(selection);
    update();
  }
}

QColor Swatch::emptyColor() const
{
  return p->emptyColor;
}

void Swatch::setEmptyColor(const QColor& emptyColor)
{
  if (emptyColor != p->emptyColor)
  {
    p->emptyColor = emptyColor;
    emptyColorChanged(emptyColor);
    update();
  }
}

} // namespace color_widgets
