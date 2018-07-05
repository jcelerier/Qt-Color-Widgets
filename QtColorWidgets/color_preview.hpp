/**
 * \file
 *
 * \author Mattia Basaglia
 *
 * \copyright Copyright (C) 2013-2015 Mattia Basaglia
 * \copyright Copyright (C) 2014 Calle Laakkonen
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
#ifndef COLOR_PREVIEW_HPP
#define COLOR_PREVIEW_HPP

#include "colorwidgets_global.hpp"

#include <QWidget>
#include <wobjectdefs.h>

namespace color_widgets {

/**
 * Simple widget that shows a preview of a color
 */
class QCP_EXPORT ColorPreview final : public QWidget
{
    W_OBJECT(ColorPreview)

public:
    enum DisplayMode
    {
        NoAlpha,    ///< Show current color with no transparency
        AllAlpha,   ///< show current color with transparency
        SplitAlpha, ///< Show both solid and transparent side by side
        SplitColor  ///< Show current and comparison colors side by side
    };
    W_ENUM(DisplayMode, NoAlpha, AllAlpha, SplitAlpha, SplitColor)

    explicit ColorPreview(QWidget *parent = 0);
    ~ColorPreview();

    /// Get the background visible under transparent colors
    QBrush background() const;

    /// Change the background visible under transparent colors
    void setBackground(const QBrush &bk);

    /// Get color display mode
    DisplayMode displayMode() const;

    /// Set how transparent colors are handled
    void setDisplayMode(DisplayMode dm);

    /// Get current color
    QColor color() const;

    /// Get the comparison color
    QColor comparisonColor() const;

    QSize sizeHint () const override;

    void paint(QPainter &painter, QRect rect) const;

    /// Set current color
    void setColor(const QColor &c); W_SLOT(setColor)

    /// Set the comparison color
    void setComparisonColor(const QColor &c); W_SLOT(setComparisonColor)

    /// Emitted when the user clicks on the widget
    void clicked() W_SIGNAL(clicked);

    /// Emitted on setColor
    void colorChanged(QColor c) W_SIGNAL(colorChanged, c);

    W_PROPERTY(QColor, color READ color WRITE setColor NOTIFY colorChanged)
    W_PROPERTY(QColor, comparisonColor READ comparisonColor WRITE setComparisonColor)
    W_PROPERTY(DisplayMode, display_mode READ displayMode WRITE setDisplayMode)
    W_PROPERTY(QBrush, background READ background WRITE setBackground)
protected:
    void paintEvent(QPaintEvent *)  override;
    void resizeEvent(QResizeEvent *)  override;
    void mouseReleaseEvent(QMouseEvent *ev)  override;
    void mouseMoveEvent(QMouseEvent *ev)  override;

private:
    class Private;
    Private * const p;
};

} // namespace color_widgets

Q_DECLARE_METATYPE(color_widgets::ColorPreview::DisplayMode)
#endif // COLOR_PREVIEW_HPP
