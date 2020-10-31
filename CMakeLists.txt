cmake_minimum_required(VERSION 3.5)

project(abx-test-qt LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Multimedia REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Multimedia REQUIRED)

add_executable(abx-test-qt
  main.cpp
  widget.cpp
  widget.h
  abxtester.cpp
  abxtester.h
  trackselectdialog.cpp
  trackselectdialog.h
  trackselectdialog.ui
  bitrateselectdialog.cpp
  bitrateselectdialog.h
  bitrateselectdialog.ui
  processingdialog.cpp
  processingdialog.h
  processingdialog.ui
  widget.ui
)

target_link_libraries(abx-test-qt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(abx-test-qt PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia)
