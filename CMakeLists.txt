cmake_minimum_required(VERSION 3.16)

project(piProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_BUILD_TYPE Debug)

#find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_add_executable(apppiProject
    src/main.cpp
    src/core/Weather/WeatherData.h
    src/core/Weather/WeatherDataManager.h
    src/core/Weather/WeatherDataManager.cpp
    src/models/Weather/WeeklyWeatherModel.h
    src/models/Weather/WeeklyWeatherModel.cpp
    resources/resources.qrc
    qml/qml.qrc
)

qt_add_qml_module(apppiProject
    URI piProjectPort
    VERSION 1.0
)

set_target_properties(apppiProject PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apppiProject
    PRIVATE Qt6::Quick)

install(TARGETS apppiProject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
