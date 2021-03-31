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
#include "color_utils.hpp"

#include <QImage>
#include <QPainter>

#include <cmath>

namespace color_widgets
{
namespace detail
{

QColor color_from_lch(color_float hue, color_float chroma, color_float luma, color_float alpha)
{
  color_float h1 = hue * 6;
  color_float x = chroma * (1 - qAbs(std::fmod(h1, 2) - 1));
  QColor col;
  if (h1 >= 0 && h1 < 1)
    col = QColor::fromRgbF(chroma, x, 0);
  else if (h1 < 2)
    col = QColor::fromRgbF(x, chroma, 0);
  else if (h1 < 3)
    col = QColor::fromRgbF(0, chroma, x);
  else if (h1 < 4)
    col = QColor::fromRgbF(0, x, chroma);
  else if (h1 < 5)
    col = QColor::fromRgbF(x, 0, chroma);
  else if (h1 < 6)
    col = QColor::fromRgbF(chroma, 0, x);

  color_float m = luma - color_lumaF(col);

  return QColor::fromRgbF(
      qBound(0.0, col.redF() + m, 1.0),
      qBound(0.0, col.greenF() + m, 1.0),
      qBound(0.0, col.blueF() + m, 1.0),
      alpha);
}

QColor color_from_hsl(color_float hue, color_float sat, color_float lig, color_float alpha)
{
  color_float chroma = (1 - qAbs(2 * lig - 1)) * sat;
  color_float h1 = hue * 6;
  color_float x = chroma * (1 - qAbs(std::fmod(h1, 2) - 1));
  QColor col;
  if (h1 >= 0 && h1 < 1)
    col = QColor::fromRgbF(chroma, x, 0);
  else if (h1 < 2)
    col = QColor::fromRgbF(x, chroma, 0);
  else if (h1 < 3)
    col = QColor::fromRgbF(0, chroma, x);
  else if (h1 < 4)
    col = QColor::fromRgbF(0, x, chroma);
  else if (h1 < 5)
    col = QColor::fromRgbF(x, 0, chroma);
  else if (h1 < 6)
    col = QColor::fromRgbF(chroma, 0, x);

  color_float m = lig - chroma / 2;

  return QColor::fromRgbF(
      qBound(0.0, col.redF() + m, 1.0),
      qBound(0.0, col.greenF() + m, 1.0),
      qBound(0.0, col.blueF() + m, 1.0),
      alpha);
}

QPixmap alpha_pixmap()
{
  QImage im(32, 32, QImage::Format_ARGB32);
  {
    QPainter p{&im};
    p.fillRect(0, 0, 16, 16, Qt::lightGray);
    p.fillRect(16, 0, 16, 16, Qt::darkGray);
    p.fillRect(0, 16, 16, 16, Qt::darkGray);
    p.fillRect(16, 16, 16, 16, Qt::lightGray);
  }
  return QPixmap::fromImage(im);
}

} // namespace detail
} // namespace color_widgets
