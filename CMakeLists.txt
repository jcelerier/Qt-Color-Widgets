#
# Copyright (C) 2015 Mattia Basaglia
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
cmake_minimum_required(VERSION 3.2)
set(COLOR_WIDGETS_LIBRARY ColorWidgets-qt5)
set(PROJECT_VERSION "")
project(${COLOR_WIDGETS_LIBRARY} CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Qt
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

# Sources
set(SOURCES
src/color_list_widget.cpp
src/gradient_slider.cpp
src/color_line_edit.cpp
src/swatch.cpp
src/bound_color_selector.cpp
src/color_palette_widget.cpp
src/color_palette_model.cpp
src/color_2d_slider.cpp
src/abstract_widget_list.cpp
src/color_delegate.cpp
src/color_widgets.qrc
src/color_utils.cpp
src/color_dialog.cpp
src/color_palette.cpp
src/color_selector.cpp
src/color_preview.cpp
src/alphaback.png
src/color_utils.hpp
src/hue_slider.cpp
src/color_wheel.cpp
src/color_names.cpp
)

set(HEADERS
QtColorWidgets/color_palette_model.hpp
QtColorWidgets/color_palette_widget.hpp
QtColorWidgets/colorwidgets_global.hpp
QtColorWidgets/color_wheel.hpp
QtColorWidgets/color_line_edit.hpp
QtColorWidgets/swatch.hpp
QtColorWidgets/color_delegate.hpp
QtColorWidgets/color_palette.hpp
QtColorWidgets/hue_slider.hpp
QtColorWidgets/abstract_widget_list.hpp
QtColorWidgets/bound_color_selector.hpp
QtColorWidgets/color_dialog.hpp
QtColorWidgets/color_2d_slider.hpp
QtColorWidgets/color_selector.hpp
QtColorWidgets/color_list_widget.hpp
QtColorWidgets/color_preview.hpp
QtColorWidgets/gradient_slider.hpp
QtColorWidgets/color_names.hpp
)

# Library

add_library(${COLOR_WIDGETS_LIBRARY} STATIC
  ${SOURCES}
  ${HEADERS})

generate_export_header(${COLOR_WIDGETS_LIBRARY})
target_compile_definitions(${COLOR_WIDGETS_LIBRARY} PRIVATE QTCOLORWIDGETS_LIBRARY)
target_link_libraries(${COLOR_WIDGETS_LIBRARY} PUBLIC Qt5::Widgets)
target_include_directories(${COLOR_WIDGETS_LIBRARY}
  PRIVATE "${PROJECT_SOURCE_DIR}/QtColorWidgets"
  PUBLIC "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")
set_target_properties(${COLOR_WIDGETS_LIBRARY} PROPERTIES
  CXX_STANDARD 14)
if(EXAMPLES)
  add_subdirectory(gallery)

  find_package(Qt5Designer QUIET)
  if(Qt5Designer_FOUND)
  add_subdirectory(color_widgets_designer_plugin)
  endif()
endif()

set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC OFF)

if(NOT MSVC AND NOT ${DISABLE_COTIRE})
cotire(${COLOR_WIDGETS_LIBRARY})
endif()
