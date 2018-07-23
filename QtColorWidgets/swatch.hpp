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
#ifndef COLOR_WIDGETS_SWATCH_HPP
#define COLOR_WIDGETS_SWATCH_HPP

#include <QWidget>
#include <QPen>
#include "color_palette.hpp"

namespace color_widgets {

/**
 * \brief A widget drawing a palette
 */
class Q_DECL_EXPORT Swatch final : public QWidget
{
    W_OBJECT(Swatch)
public:
    enum ColorSizePolicy
    {
        Hint,    ///< The size is just a hint
        Minimum, ///< Can expand but not contract
        Fixed    ///< Must be exactly as specified
    };
    Q_ENUMS(ColorSizePolicy)

    Swatch(QWidget* parent = nullptr);
    ~Swatch() override;

    QSize sizeHint() const Q_DECL_OVERRIDE;
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

    const ColorPalette& palette() const;
    ColorPalette& palette();
    int selected() const;
    /**
     * \brief Color at the currently selected index
     */
    QColor selectedColor() const;

    /**
     * \brief Color index at the given position within the widget
     * \param p Point in local coordinates
     * \returns -1 if the position doesn't represent any color
     */
    int indexAt(const QPoint& p);

    /**
     * \brief Color at the given position within the widget
     * \param p Point in local coordinates
     */
    QColor colorAt(const QPoint& p);

    QSize colorSize() const;
    ColorSizePolicy colorSizePolicy() const;
    QPen border() const;
    QPen selection() const;
    int margin() const;
    QColor emptyColor() const;

    int forcedRows() const;
    int forcedColumns() const;

    bool readOnly() const;

    void setPalette(const ColorPalette& palette); W_SLOT(setPalette)
    void setSelected(int selected); W_SLOT(setSelected)
    void clearSelection(); W_SLOT(clearSelection)
    void setColorSize(const QSize& colorSize); W_SLOT(setColorSize)
    void setColorSizePolicy(ColorSizePolicy colorSizePolicy); W_SLOT(setColorSizePolicy)
    void setBorder(const QPen& border); W_SLOT(setBorder)
    void setSelection(const QPen& selection); W_SLOT(setSelection)
    void setMargin(const int& margin); W_SLOT(setMargin)
    void setEmptyColor(const QColor& emptyColor); W_SLOT(setEmptyColor)
    void setForcedRows(int forcedRows); W_SLOT(setForcedRows)
    void setForcedColumns(int forcedColumns); W_SLOT(setForcedColumns)
    void setReadOnly(bool readOnly); W_SLOT(setReadOnly)
    /**
     * \brief Remove the currently seleceted color
     **/
    void removeSelected();

    void paletteChanged(const ColorPalette& palette) W_SIGNAL(paletteChanged, palette);
    void selectedChanged(int selected)  W_SIGNAL(selectedChanged, selected);
    void colorSelected(const QColor& color) W_SIGNAL(colorSelected, color);
    void colorSizeChanged(const QSize& colorSize) W_SIGNAL(colorSizeChanged, colorSize);
    void colorSizePolicyChanged(ColorSizePolicy colorSizePolicy) W_SIGNAL(colorSizePolicyChanged, colorSizePolicy);
    void doubleClicked(int index) W_SIGNAL(doubleClicked, index);
    void rightClicked(int index) W_SIGNAL(rightClicked, index);
    void forcedRowsChanged(int forcedRows) W_SIGNAL(forcedRowsChanged, forcedRows);
    void forcedColumnsChanged(int forcedColumns) W_SIGNAL(forcedColumnsChanged, forcedColumns);
    void readOnlyChanged(bool readOnly) W_SIGNAL(readOnlyChanged, readOnly);
    void borderChanged(const QPen& border) W_SIGNAL(borderChanged, border);
    void selectionChanged(const QPen& selection) W_SIGNAL(selectionChanged, selection);
    void marginChanged(const int& margin) W_SIGNAL(marginChanged, margin);
    void emptyColorChanged(const QColor& emptyColor) W_SIGNAL(emptyColorChanged, emptyColor);

protected:
    bool event(QEvent* event) Q_DECL_OVERRIDE;

    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;

    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;

    /**
     * \brief Connected to the internal palette object to keep eveything consistent
     */
    void paletteModified(); W_SLOT(paletteModified)

    /**
     * \brief Palette shown by the widget
     */
    //W_PROPERTY(const ColorPalette&, palette READ palette WRITE setPalette NOTIFY paletteChanged)
    /**
     * \brief Currently selected color (-1 if no color is selected)
     */
    W_PROPERTY(int, selected READ selected WRITE setSelected NOTIFY selectedChanged)

    /**
     * \brief Preferred size for a color square
     */
    W_PROPERTY(QSize, colorSize READ colorSize WRITE setColorSize NOTIFY colorSizeChanged)

    W_PROPERTY(ColorSizePolicy, colorSizePolicy READ colorSizePolicy WRITE setColorSizePolicy NOTIFY colorSizePolicyChanged)

    /**
     * \brief Border around the colors
     */
    W_PROPERTY(QPen, border READ border WRITE setBorder NOTIFY borderChanged)

    /**
     * \brief Selection border of the color
     */
    W_PROPERTY(QPen, selection READ selection WRITE setSelection NOTIFY selectionChanged)

   /**
    * \brief Set the empty color for the swatch
    */
    W_PROPERTY(QColor, emptyColor READ emptyColor WRITE setEmptyColor NOTIFY emptyColorChanged)

   /**
    * \brief Margin around the colors
    */
    W_PROPERTY(int, margin READ margin WRITE setMargin NOTIFY marginChanged)

    /**
     * \brief Forces the Swatch to display that many rows of colors
     *
     * If there are too few elements, the widget will display less than this
     * many rows.
     *
     * A value of0 means that the number of rows is automatic.
     *
     * \note Conflicts with forcedColumns
     */
     W_PROPERTY(int, forcedRows READ forcedRows WRITE setForcedRows NOTIFY forcedRowsChanged)

    /**
     * \brief Forces the Swatch to display that many columns of colors
     *
     * If there are too few elements, the widget will display less than this
     * many columns.
     *
     * A value of 0 means that the number of columns is automatic.
     *
     * \note Conflicts with forcedRows
     */
    W_PROPERTY(int, forcedColumns READ forcedColumns WRITE setForcedColumns NOTIFY forcedColumnsChanged)

    /**
     * \brief Whether the palette can be modified via user interaction
     * \note Even when this is \b false, it can still be altered programmatically
     */
    W_PROPERTY(bool, readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)

private:
    class Private;
    Private* p;
};


} // namespace color_widgets
W_REGISTER_ARGTYPE(color_widgets::ColorPalette)
W_REGISTER_ARGTYPE(QColor)
W_REGISTER_ARGTYPE(QPen)
W_REGISTER_ARGTYPE(color_widgets::Swatch::ColorSizePolicy)
#endif // COLOR_WIDGETS_SWATCH_HPP
