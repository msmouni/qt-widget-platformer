cmake_minimum_required(VERSION 3.5)

project(qt-widget-platformer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        app.cpp
        app.h
        app.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qt-widget-platformer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        play.h play.cpp
        character.h character.cpp
        player.h player.cpp
        res_files.qrc
        platform.h platform.cpp
        csv.h
        tile.h tile.cpp
        animation.h animation.cpp
        enemy.h enemy.cpp
        find.h find.cpp
        path_finding.h path_finding.cpp
        a_star.h a_star.cpp
        weapon.h weapon.cpp
        bomb.h bomb.cpp
        collision.h collision.cpp
        dynamic.h dynamic.cpp
        progressbar.h progressbar.cpp
    )

# https://stackoverflow.com/questions/18471827/qtcreator-cant-find-the-class-header-file-after-promoting-a-widget-to-that-clas
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET qt-widget-platformer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(qt-widget-platformer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(qt-widget-platformer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(qt-widget-platformer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.qt-widget-platformer)
endif()
set_target_properties(qt-widget-platformer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS qt-widget-platformer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qt-widget-platformer)
endif()
