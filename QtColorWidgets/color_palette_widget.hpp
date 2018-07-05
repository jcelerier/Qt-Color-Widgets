/**
 * \file
 *
 * \author Mattia Basaglia
 *
 * \copyright Copyright (C) 2013-2015 Mattia Basaglia
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
#ifndef COLOR_WIDGETS_COLOR_PALETTE_WIDGET_HPP
#define COLOR_WIDGETS_COLOR_PALETTE_WIDGET_HPP

#include <memory>
#include <QWidget>
#include "color_palette_model.hpp"
#include "swatch.hpp"

namespace color_widgets {

/**
 * \brief A widget to use and modify palettes
 */
class Q_DECL_EXPORT ColorPaletteWidget final : public QWidget
{
    W_OBJECT(ColorPaletteWidget)

public:
    ColorPaletteWidget(QWidget* parent = nullptr);
    ~ColorPaletteWidget();

    ColorPaletteModel* model() const;

    /**
     * \brief Currently selected palette
     * \pre model() != nullptr and there is a selected palette
     */
    const ColorPalette& currentPalette() const;

    QSize colorSize() const;
    Swatch::ColorSizePolicy colorSizePolicy() const;
    QPen border() const;

    int forcedRows() const;
    int forcedColumns() const;

    bool readOnly() const;
    QColor currentColor() const;

    int currentRow() const;

    void setModel(ColorPaletteModel* model); W_SLOT(setModel)
    void setColorSize(const QSize& colorSize); W_SLOT(setColorSize)
    void setColorSizePolicy(Swatch::ColorSizePolicy colorSizePolicy); W_SLOT(setColorSizePolicy)
    void setBorder(const QPen& border); W_SLOT(setBorder)
    void setForcedRows(int forcedRows); W_SLOT(setForcedRows)
    void setForcedColumns(int forcedColumns); W_SLOT(setForcedColumns)
    void setReadOnly(bool readOnly); W_SLOT(setReadOnly)
    /**
     * \brief Clear the selected color
     */
    void clearCurrentColor();
    /**
     * \brief Attempt to select a color
     *
     * If the given color is available in the current palete, it will be selected
     * \return \b true on success
     */
    bool setCurrentColor(const QColor& color);

    void setCurrentRow(int currentRow);

    void modelChanged(ColorPaletteModel* model) W_SIGNAL(modelChanged, model);
    void colorSizeChanged(const QSize& colorSize) W_SIGNAL(colorSizeChanged, colorSize);
    void colorSizePolicyChanged(Swatch::ColorSizePolicy colorSizePolicy) W_SIGNAL(colorSizePolicyChanged, colorSizePolicy);
    void forcedRowsChanged(int forcedRows) W_SIGNAL(forcedRowsChanged, forcedRows);
    void forcedColumnsChanged(int forcedColumns) W_SIGNAL(forcedColumnsChanged, forcedColumns);
    void readOnlyChanged(bool readOnly) W_SIGNAL(readOnlyChanged, readOnly);
    void currentColorChanged(const QColor& currentColor) W_SIGNAL(currentColorChanged, currentColor);
    void selectedChanged(int index) W_SIGNAL(selectedChanged, index);
    void borderChanged(const QPen& border) W_SIGNAL(borderChanged, border);
    void currentRowChanged(int currentRow) W_SIGNAL(currentRowChanged, currentRow);

    void on_palette_list_currentIndexChanged(int index); W_SLOT(on_palette_list_currentIndexChanged)
    void on_swatch_doubleClicked(int index); W_SLOT(on_swatch_doubleClicked)

    /**
     * \brief Model used to store the palettes
     */
    W_PROPERTY(ColorPaletteModel*, model READ model WRITE setModel NOTIFY modelChanged)

    /**
     * \brief Size of a single color in the swatch widget
     */
    W_PROPERTY(QSize, colorSize READ colorSize WRITE setColorSize NOTIFY colorSizeChanged)
    /**
     * \brief Policy for colorSize
     **/
    W_PROPERTY(color_widgets::Swatch::ColorSizePolicy, colorSizePolicy READ colorSizePolicy WRITE setColorSizePolicy NOTIFY colorSizePolicyChanged)

    /**
     * \brief Border around the colors
     */
    W_PROPERTY(QPen, border READ border WRITE setBorder NOTIFY borderChanged)

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
     * \brief Whether the palettes can be modified via user interaction
     */
    W_PROPERTY(bool, readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)

    /**
     * \brief Currently selected color
     */
    W_PROPERTY(QColor, currentColor READ currentColor WRITE setCurrentColor NOTIFY currentColorChanged)

    /**
     * \brief Currently selected model row
     */
    W_PROPERTY(int, currentRow READ currentRow WRITE setCurrentRow  NOTIFY currentRowChanged)

private:
    class Private;
    std::unique_ptr<Private> p;
};

} // namespace color_widgets
#endif // COLOR_WIDGETS_COLOR_PALETTE_WIDGET_HPP

