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
#include "gradient_slider.hpp"
#include "color_utils.hpp"

#include <QPainter>
#include <QStyleOptionSlider>
#include <QLinearGradient>
#include <wobjectimpl.h>
W_OBJECT_IMPL(color_widgets::GradientSlider)
namespace color_widgets {

class GradientSlider::Private
{
public:
    QLinearGradient gradient;
    QBrush back;
    int verticalSpacing;
    QPen border;

    Private() :
        back(Qt::darkGray, Qt::DiagCrossPattern),
        verticalSpacing(0),
        border(Qt::NoPen)
    {
        back.setTexture(detail::alpha_pixmap());
        gradient.setCoordinateMode(QGradient::StretchToDeviceMode);
    }

};

GradientSlider::GradientSlider(QWidget *parent) :
    QSlider(Qt::Horizontal, parent), p(new Private)
{}

GradientSlider::GradientSlider(Qt::Orientation orientation, QWidget *parent) :
    QSlider(orientation, parent), p(new Private)
{}

GradientSlider::~GradientSlider()
{
    delete p;
}

QBrush GradientSlider::background() const
{
    return p->back;
}

void GradientSlider::setBackground(const QBrush &bg)
{
    p->back = bg;
    update();
}

int GradientSlider::verticalSpacing() const
{
    return p->verticalSpacing;
}

void GradientSlider::setVerticalSpacing(const int &verticalSpacing)
{
    p->verticalSpacing = verticalSpacing;
    update();
}

QPen GradientSlider::border() const
{
    return p->border;
}

void GradientSlider::setBorder(const QPen &border)
{
    p->border = border;
    update();
}


QGradientStops GradientSlider::colors() const
{
    return p->gradient.stops();
}

void GradientSlider::setColors(const QGradientStops &colors)
{
    p->gradient.setStops(colors);
    update();
}

QLinearGradient GradientSlider::gradient() const
{
    return p->gradient;
}

void GradientSlider::setGradient(const QLinearGradient &gradient)
{
    p->gradient = gradient;
    update();
}

void GradientSlider::setFirstColor(const QColor &c)
{
    QGradientStops stops = p->gradient.stops();
    if(stops.isEmpty())
        stops.push_back(QGradientStop(0.0, c));
    else
        stops.front().second = c;
    p->gradient.setStops(stops);

    update();
}

void GradientSlider::setLastColor(const QColor &c)
{
    QGradientStops stops = p->gradient.stops();
    if(stops.size()<2)
        stops.push_back(QGradientStop(1.0, c));
    else
        stops.back().second = c;
    p->gradient.setStops(stops);
    update();
}

QColor GradientSlider::firstColor() const
{
    QGradientStops s = colors();
    return s.empty() ? QColor() : s.front().second;
}

QColor GradientSlider::lastColor() const
{
    QGradientStops s = colors();
    return s.empty() ? QColor() : s.back().second;
}

void GradientSlider::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QStyleOptionFrame panel;
    panel.initFrom(this);
    panel.lineWidth = 1;
    panel.midLineWidth = 0;
    panel.state |= QStyle::State_Sunken;
    style()->drawPrimitive(QStyle::PE_Frame, &panel, &painter, this);
    QRect r = style()->subElementRect(QStyle::SE_FrameContents, &panel, this);
    painter.setClipRect(r);

    if(orientation() == Qt::Horizontal)
        p->gradient.setFinalStop(1, 0);
    else
        p->gradient.setFinalStop(0, 1);

    painter.setPen(p->border);
    painter.setBrush(p->back);
    painter.drawRect(1+p->border.width(),1+p->border.width()+p->verticalSpacing,geometry().width()-2-p->border.width()*2,geometry().height()-2-p->verticalSpacing*2-p->border.width()*2);
    painter.setBrush(p->gradient);
    painter.drawRect(1+p->border.width(),1+p->border.width()+p->verticalSpacing,geometry().width()-2-p->border.width()*2,geometry().height()-2-p->verticalSpacing*2-p->border.width()*2);

    painter.setClipping(false);
    QStyleOptionSlider opt_slider;
    initStyleOption(&opt_slider);
    opt_slider.state &= ~QStyle::State_HasFocus;
    opt_slider.subControls = QStyle::SC_SliderHandle;
    if (isSliderDown())
    {
        opt_slider.state |= QStyle::State_Sunken;
        opt_slider.activeSubControls = QStyle::SC_SliderHandle;
    }
    opt_slider.rect = style()->subControlRect(QStyle::CC_Slider,&opt_slider,
                                              QStyle::SC_SliderHandle,this);

    style()->drawComplexControl(QStyle::CC_Slider, &opt_slider, &painter, this);
}

} // namespace color_widgets
