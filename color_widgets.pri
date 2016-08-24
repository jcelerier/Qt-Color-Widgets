# Copyright (C) 2013-2015 Mattia Basaglia
#
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Color Widgets.  If not, see <http://www.gnu.org/licenses/>.

CONFIG += c++11

INCLUDEPATH += $$PWD/src $$PWD/include

SOURCES += \
    $$PWD/src/color_preview.cpp \
    $$PWD/src/color_wheel.cpp \
    $$PWD/src/gradient_slider.cpp \
    $$PWD/src/hue_slider.cpp \
    $$PWD/src/color_dialog.cpp \
    $$PWD/src/color_selector.cpp \
    $$PWD/src/color_delegate.cpp \
    $$PWD/src/color_list_widget.cpp \
    $$PWD/src/bound_color_selector.cpp \
    $$PWD/src/abstract_widget_list.cpp \
    $$PWD/src/color_palette.cpp \
    $$PWD/src/color_palette_model.cpp \
    $$PWD/src/color_palette_widget.cpp \
    $$PWD/src/swatch.cpp \
    $$PWD/src/color_utils.cpp \
    $$PWD/src/color_2d_slider.cpp \
    $$PWD/src/color_line_edit.cpp \
    $$PWD/src/color_names.cpp

HEADERS += \
    $$PWD/QtColorWidgets/color_wheel.hpp \
    $$PWD/QtColorWidgets/color_preview.hpp \
    $$PWD/QtColorWidgets/gradient_slider.hpp \
    $$PWD/QtColorWidgets/hue_slider.hpp \
    $$PWD/QtColorWidgets/color_dialog.hpp \
    $$PWD/QtColorWidgets/color_selector.hpp \
    $$PWD/QtColorWidgets/color_delegate.hpp \
    $$PWD/QtColorWidgets/color_list_widget.hpp \
    $$PWD/QtColorWidgets/bound_color_selector.hpp \
    $$PWD/QtColorWidgets/abstract_widget_list.hpp \
    $$PWD/QtColorWidgets/colorwidgets_global.hpp \
    $$PWD/QtColorWidgets/color_palette.hpp \
    $$PWD/QtColorWidgets/color_palette_model.hpp \
    $$PWD/QtColorWidgets/color_palette_widget.hpp \
    $$PWD/QtColorWidgets/swatch.hpp \
    $$PWD/src/color_utils.hpp \
    $$PWD/QtColorWidgets/color_2d_slider.hpp \
    $$PWD/QtColorWidgets/color_line_edit.hpp \
    $$PWD/QtColorWidgets/color_names.hpp

FORMS += \
    $$PWD/src/color_dialog.ui \
    $$PWD/src/color_palette_widget.ui

RESOURCES += \
    $$PWD/src/color_widgets.qrc

