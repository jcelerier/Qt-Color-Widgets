/**
 * \file
 *
 * \author Mattia Basaglia
 *
 * \copyright Copyright (C) 2014 Calle Laakkonen
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
#ifndef HUE_SLIDER_HPP
#define HUE_SLIDER_HPP

#include "gradient_slider.hpp"
#include <wobjectdefs.h>

namespace color_widgets {

/**
 * \brief A slider for selecting a hue value
 */
class QCP_EXPORT HueSlider : public GradientSlider
{
    W_OBJECT(HueSlider)

public:
    explicit HueSlider(QWidget *parent = nullptr);
    explicit HueSlider(Qt::Orientation orientation, QWidget *parent = nullptr);
    ~HueSlider();

    qreal colorSaturation() const;
    qreal colorValue() const;
    qreal colorAlpha() const;
    QColor color() const;
    qreal colorHue() const;

public:
    void setColorValue(qreal value);  W_SLOT(setColorValue)
    void setColorSaturation(qreal value);  W_SLOT(setColorSaturation)
    void setColorAlpha(qreal alpha);  W_SLOT(setColorAlpha)
    void setColorHue(qreal colorHue);  W_SLOT(setColorHue)
    /**
     * \brief Set Hue Saturation and ColorValue, ignoring alpha
     */
    void setColor(const QColor& color);  W_SLOT(setColor)
    /**
     * \brief Set Hue Saturation, ColorValue and Alpha
     */
    void setFullColor(const QColor& color); W_SLOT(setFullColor)

    void colorHueChanged(qreal colorHue) W_SIGNAL(colorHueChanged, colorHue);

    /**
     * \brief Saturation used in the rainbow gradient, as a [0-1] float
     */
    W_PROPERTY(qreal, colorSaturation READ colorSaturation WRITE setColorSaturation)
    /**
     * \brief Value used in the rainbow gradient, as a [0-1] float
     */
    W_PROPERTY(qreal, colorValue READ colorValue WRITE setColorValue)
    /**
     * \brief Alpha used in the rainbow gradient, as a [0-1] float
     */
    W_PROPERTY(qreal, colorAlpha READ colorAlpha WRITE setColorAlpha)

    /**
     * \brief Color with corresponding color* components
     */
    W_PROPERTY(QColor, color READ color WRITE setColor)

    /**
     * \brief Normalized Hue, as indicated from the slider
     */
    W_PROPERTY(qreal, colorHue READ colorHue WRITE setColorHue NOTIFY colorHueChanged)

private:
    class Private;
    Private * const p;
};

} // namespace color_widgets

#endif // HUE_SLIDER_HPP

