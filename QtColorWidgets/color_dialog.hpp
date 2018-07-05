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
#ifndef COLOR_DIALOG_HPP
#define COLOR_DIALOG_HPP

#include "colorwidgets_global.hpp"
#include "color_preview.hpp"
#include "color_wheel.hpp"

#include <QDialog>
#include <wobjectdefs.h>
class QAbstractButton;

namespace color_widgets {

class QCP_EXPORT ColorDialog final : public QDialog
{
    W_OBJECT(ColorDialog)

public:
    enum ButtonMode {
        OkCancel,
        OkApplyCancel,
        Close
    };
    W_ENUM(ButtonMode, OkCancel, OkApplyCancel, Close)
    explicit ColorDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);

    /**
     * Get currently selected color
     */
    QColor color() const;

    /**
     * Set the display mode for the color preview
     */
    void setPreviewDisplayMode(ColorPreview::DisplayMode mode);

    /**
     * Get the color preview diplay mode
     */
    ColorPreview::DisplayMode previewDisplayMode() const;

    bool alphaEnabled() const;

    /**
     * Select which dialog buttons to show
     *
     * There are three predefined modes:
     * OkCancel - this is useful when the dialog is modal and we just want to return a color
     * OkCancelApply - this is for non-modal dialogs
     * Close - for non-modal dialogs with direct color updates via colorChanged signal
     */
    void setButtonMode(ButtonMode mode);
    ButtonMode buttonMode() const;

    QSize sizeHint() const  override;

    ColorWheel::DisplayFlags wheelFlags() const;


    /**
     * Change color
     */
    void setColor(const QColor &c); W_SLOT(setColor)

  /**
     * Set the current color and show the dialog
     */
    void showColor(const QColor &oldcolor); W_SLOT(showColor)

    void setWheelFlags(ColorWheel::DisplayFlags flags);  W_SLOT(setWheelFlags)

    /**
     * Set whether the color alpha channel can be edited.
     * If alpha is disabled, the selected color's alpha will always be 255.
     */
    void setAlphaEnabled(bool a); W_SLOT(setAlphaEnabled)

    /**
     * The current color was changed
     */
    void colorChanged(QColor c) W_SIGNAL(colorChanged, c)

    /**
     * The user selected the new color by pressing Ok/Apply
     */
    void colorSelected(QColor c) W_SIGNAL(colorSelected, c)

    void wheelFlagsChanged(ColorWheel::DisplayFlags flags) W_SIGNAL(wheelFlagsChanged, flags)
    void alphaEnabledChanged(bool alphaEnabled) W_SIGNAL(alphaEnabledChanged, alphaEnabled)

private Q_SLOTS:
    /// Update all the Ui elements to match the selected color
    void update_widgets();W_SLOT(update_widgets)

    /// Update from HSV sliders
    void set_hsv();W_SLOT(set_hsv)

    /// Update from RGB sliders
    void set_rgb();W_SLOT(set_rgb)


    void on_edit_hex_colorChanged(const QColor& color);W_SLOT(on_edit_hex_colorChanged)

    void on_edit_hex_colorEditingFinished(const QColor& color);W_SLOT(on_edit_hex_colorEditingFinished)


    void on_buttonBox_clicked(QAbstractButton*); W_SLOT(on_buttonBox_clicked)

private:
    void setColorInternal(const QColor &color);

protected:
    void dragEnterEvent(QDragEnterEvent *event)  override;
    void dropEvent(QDropEvent * event)  override;
    void mouseReleaseEvent(QMouseEvent *event)  override;
    void mouseMoveEvent(QMouseEvent *event)  override;

    W_PROPERTY(QColor, color READ color WRITE setColor NOTIFY colorChanged)
    W_PROPERTY(ColorWheel::DisplayFlags, wheelFlags READ wheelFlags WRITE setWheelFlags NOTIFY wheelFlagsChanged)
    /**
     * \brief whether the color alpha channel can be edited.
     *
     * If alpha is disabled, the selected color's alpha will always be 255.
     */
    W_PROPERTY(bool, alphaEnabled READ alphaEnabled WRITE setAlphaEnabled NOTIFY alphaEnabledChanged)
private:
    class Private;
    Private * const p;
};

} // namespace color_widgets

W_REGISTER_ARGTYPE(QAbstractButton*)
#endif // COLOR_DIALOG_HPP
