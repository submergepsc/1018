cmake_minimum_required(VERSION 3.16)
project(PolyCalculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
endif()

set(SOURCES
    main.cpp
    mainwindow.cpp
    AlgorithmExpression.cpp
    polynomial.cpp
)

set(HEADERS
    mainwindow.h
    AlgorithmExpression.h
    polynomial.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} mainwindow.ui)

if(Qt6_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
