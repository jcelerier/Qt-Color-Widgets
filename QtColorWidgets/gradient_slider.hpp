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
#ifndef GRADIENT_SLIDER_HPP
#define GRADIENT_SLIDER_HPP

#include "colorwidgets_global.hpp"

#include <QSlider>
#include <QPen>
#include <QGradient>
#include <wobjectdefs.h>
namespace color_widgets {

/**
 * \brief A slider that mover on top of a gradient
 */
class QCP_EXPORT GradientSlider : public QSlider
{
    W_OBJECT(GradientSlider)
public:
    explicit GradientSlider(QWidget *parent = 0);
    explicit GradientSlider(Qt::Orientation orientation, QWidget *parent = 0);
    ~GradientSlider() override;

    /// Get the background, it's visible for transparent gradient stops
    QBrush background() const;
    /// Set the background, it's visible for transparent gradient stops
    void setBackground(const QBrush &bg);

    /// Get the vertical spacing.
    int verticalSpacing() const;
    /// Set the vertical spacing.
    void setVerticalSpacing(const int &verticalSpacing);

    /// Get the border pen.
    QPen border() const;
    /// Set the border pen.
    void setBorder(const QPen &border);

    /// Get the colors that make up the gradient
    QGradientStops colors() const;
    /// Set the colors that make up the gradient
    void setColors(const QGradientStops &colors);

    /// Get the gradient
    QLinearGradient gradient() const;
    /// Set the gradient
    void setGradient(const QLinearGradient &gradient);

    /**
     * \brief Set the first color of the gradient
     *
     * If the gradient is currently empty it will create a stop with the given color
     */
    void setFirstColor(const QColor &c);

    /**
     * \brief Set the last color of the gradient
     *
     * If the gradient is has less than two colors,
     * it will create a stop with the given color
     */
    void setLastColor(const QColor &c);

    /**
     * \brief Get the first color
     *
     * \returns QColor() con empty gradient
     */
    QColor firstColor() const;

    /**
     * \brief Get the last color
     *
     * \returns QColor() con empty gradient
     */
    QColor lastColor() const;

    W_PROPERTY(QBrush, background READ background WRITE setBackground)
    W_PROPERTY(int, verticalSpacing READ verticalSpacing WRITE setVerticalSpacing)
    W_PROPERTY(QPen, border READ border WRITE setBorder)
    W_PROPERTY(QLinearGradient, gradient READ gradient WRITE setGradient)
    W_PROPERTY(QColor, firstColor READ firstColor WRITE setFirstColor)
    W_PROPERTY(QColor, lastColor READ lastColor WRITE setLastColor)
    W_PROPERTY(QGradientStops, colors READ colors WRITE setColors)

protected:
    void paintEvent(QPaintEvent *ev)  override;

private:
    class Private;
    Private * const p;
};

} // namespace color_widgets

#endif // GRADIENT_SLIDER_HPP
