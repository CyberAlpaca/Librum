cmake_minimum_required(VERSION 3.16)

project(GuiTests LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Gui Quick Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Qml Quick Test Concurrent)
find_package(Qt6 REQUIRED COMPONENTS QuickControls2)

add_subdirectory(suri)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RESOURCE_FILE_PATH ${CMAKE_SOURCE_DIR}/src/presentation/qmlSources.qrc)
message("===== RESOURCE_FILE_PATH: " ${CMAKE_SOURCE_DIR})
qt6_add_resources(RESOURCES ${RESOURCE_FILE_PATH})
set(INITAPP_PATH ${CMAKE_SOURCE_DIR}/src/application_init.cpp)
set(GLOB_CONTROLLERS_PATH ${CMAKE_SOURCE_DIR}/src/global_controllers.cpp)

add_executable(GuiTests
    tst_guitests.cpp
    ${INITAPP_PATH}
    ${GLOB_CONTROLLERS_PATH}
    ${RESOURCES}
)
add_test(NAME GuiTests COMMAND GuiTests)

if(UNIX)
    set(MUPDF_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/libs/mupdf/build/$<IF:$<CONFIG:Debug>,shared-debug,shared-release>")
    set(MUPDF_OUTPUT "${MUPDF_OUTPUT_DIR}/libmupdfcpp.so")
    set(MUPDF_OUTPUT "${MUPDF_OUTPUT_DIR}/libmupdfcpp.so" PARENT_SCOPE)
elseif(WIN32)
    set(MUPDF_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/libs/mupdf/platform/win32/x64/$<IF:$<CONFIG:Debug>,Debug,Release>")
    set(MUPDF_OUTPUT "${MUPDF_OUTPUT_DIR}/mupdfcpp64.lib")
    set(MUPDF_OUTPUT "${MUPDF_OUTPUT_DIR}/mupdfcpp64.lib" PARENT_SCOPE)
endif()


target_link_libraries(GuiTests
    PUBLIC
        rapidfuzz::rapidfuzz
    PRIVATE
        presentation
        adapters
        application
        infrastructure
        domain
        Qt6::Concurrent
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::Qml
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Test
        Qt${QT_VERSION_MAJOR}::QuickControls2
        ${MUPDF_OUTPUT}
        suri
)

target_include_directories(GuiTests
                PRIVATE
                    ${ADAPTERS_INCLUDES}
                    ${APPLICATION_INCLUDES}
                    ${DOMAIN_INCLUDES}
                    ${INFRASTRUCTURE_INCLUDES}
                    ${CMAKE_CURRENT_SOURCE_DIR}/services
                    ${CMAKE_CURRENT_SOURCE_DIR}/utility
)

# Define QT_GUI_LIB
target_compile_definitions(GuiTests PRIVATE QT_GUI_LIB)
