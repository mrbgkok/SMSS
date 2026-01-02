cmake_minimum_required(VERSION 3.16)

project(SMSS
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/database.cpp
    src/database.h
    ui/mainwindow.ui
)

if(WIN32)
    set(APP_ICON_RC "${CMAKE_CURRENT_SOURCE_DIR}/resources/smss.rc")
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(SMSS
        ${PROJECT_SOURCES}
        ${APP_ICON_RC}
    )
else()
    add_executable(SMSS
        ${PROJECT_SOURCES}
        ${APP_ICON_RC}
    )
endif()


target_link_libraries(SMSS
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Sql
)


if(WIN32)
    set_target_properties(SMSS PROPERTIES
        WIN32_EXECUTABLE TRUE
        AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui"
    )
endif()

if(APPLE)
    set_target_properties(SMSS PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )
endif()

include(GNUInstallDirs)
install(TARGETS SMSS
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE  DESTINATION .
)

