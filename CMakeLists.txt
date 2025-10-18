cmake_minimum_required(VERSION 3.16)
project(PolynomialCalculator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(PolynomialCalculator
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    polynomial/polynomial.cpp
    polynomial/polynomial.h
)

target_include_directories(PolynomialCalculator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(PolynomialCalculator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
