cmake_minimum_required(VERSION 3.16)
project(StatisticsVisualizer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB SOURCE_FILES
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.h"
)

set(RESOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc
)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/StatisticsVisualizer_ru_RU.ts
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Charts LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Charts LinguistTools)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(StatisticsVisualizer MANUAL_FINALIZATION
        ${SOURCE_FILES}
        ${RESOURCE_FILES}
        ${TS_FILES}
    )
    qt_create_translation(QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES})
else()
    add_executable(StatisticsVisualizer
        ${SOURCE_FILES}
        ${RESOURCE_FILES}
        ${TS_FILES}
    )
    qt5_create_translation(QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES})
endif()

# Исправленный вызов target_link_libraries с ключевым словом
target_link_libraries(StatisticsVisualizer
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Charts
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.StatisticsVisualizer)
endif()
set_target_properties(StatisticsVisualizer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS StatisticsVisualizer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(StatisticsVisualizer)
endif()
