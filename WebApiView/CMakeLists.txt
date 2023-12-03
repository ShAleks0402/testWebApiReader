cmake_minimum_required(VERSION 3.5)

project(WebApiView VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Qml Quick Gui QuickWidgets Sql Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets  Qml Quick Gui QuickWidgets Sql Network)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
        qrc.qrc

        ApiTableWidget/ApiTableWidget.h
        ApiTableWidget/ApiTableWidget.cpp
        ApiTableWidget/ApiTableModel.h
        ApiTableWidget/ApiTableModel.cpp

        WebReader.h
        WebReader.cpp

        DataBase.h
        DataBase.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(WebApiView
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET WebApiView APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(WebApiView SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(WebApiView
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(WebApiView PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(WebApiView PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(WebApiView PRIVATE Qt${QT_VERSION_MAJOR}::Qml)
target_link_libraries(WebApiView PRIVATE Qt${QT_VERSION_MAJOR}::Quick)
target_link_libraries(WebApiView PRIVATE Qt${QT_VERSION_MAJOR}::QuickWidgets)
target_link_libraries(WebApiView PRIVATE Qt${QT_VERSION_MAJOR}::Sql)
target_link_libraries(WebApiView PRIVATE Qt${QT_VERSION_MAJOR}::Network)

#link_directories("~Work/openssl-1.1.1q -lcrypto -L~Work/openssl-1.1.1q -lssl")
#OPENSSL_LINKING_MODE = "-linked"

set_target_properties(WebApiView PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS WebApiView
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(WebApiView)
endif()
