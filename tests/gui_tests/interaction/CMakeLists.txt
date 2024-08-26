cmake_minimum_required(VERSION 3.16)

project(InteractionTests LANGUAGES CXX)

enable_testing()

find_package(Qt6 REQUIRED COMPONENTS Quick Qml QuickControls2 Test Concurrent)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(RESOURCE_FILE_PATH ${CMAKE_SOURCE_DIR}/src/presentation/qmlSources.qrc)
qt6_add_resources(RESOURCES ${RESOURCE_FILE_PATH})

if(UNIX)
    set(MUPDF_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/libs/mupdf/build/$<IF:$<CONFIG:Debug>,shared-debug,shared-release>")
    set(MUPDF_OUTPUT "${MUPDF_OUTPUT_DIR}/libmupdfcpp.so")
    set(MUPDF_OUTPUT "${MUPDF_OUTPUT_DIR}/libmupdfcpp.so" PARENT_SCOPE)
elseif(WIN32)
    set(MUPDF_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/libs/mupdf/platform/win32/x64/$<IF:$<CONFIG:Debug>,Debug,Release>")
    set(MUPDF_OUTPUT "${MUPDF_OUTPUT_DIR}/mupdfcpp64.lib")
    set(MUPDF_OUTPUT "${MUPDF_OUTPUT_DIR}/mupdfcpp64.lib" PARENT_SCOPE)
endif()

set(COMMON_PRV_LIBRARIES
    presentation
    adapters
    application
    infrastructure
    domain
    Qt6::Concurrent
    Qt6::Quick
    Qt6::Qml
    Qt6::Widgets
    Qt6::Test
    Qt6::QuickControls2
    ${MUPDF_OUTPUT}
    suri
)

set(COMMON_PUBLIC_LIBRARIES
    rapidfuzz::rapidfuzz
)

set(COMMON_INCLUDE_DIRS
    ${ADAPTERS_INCLUDES}
    ${APPLICATION_INCLUDES}
    ${DOMAIN_INCLUDES}
    ${INFRASTRUCTURE_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/services
    ${CMAKE_CURRENT_SOURCE_DIR}/utility
)

set(COMMON_EXE_SOURCES
    ${CMAKE_SOURCE_DIR}/src/application_init.cpp
    ${CMAKE_SOURCE_DIR}/src/global_controllers.cpp
)

# Function to add executables with common settings
function(add_test_executable target_name)
    add_executable(${target_name} ${ARGN} ${COMMON_EXE_SOURCES})
    target_include_directories(${target_name} PRIVATE ${COMMON_INCLUDE_DIRS})
    target_link_libraries(${target_name} PRIVATE ${COMMON_PRV_LIBRARIES})
    target_link_libraries(${target_name} PUBLIC ${COMMON_PUBLIC_LIBRARIES})
endfunction()

add_test_executable(InteractionTests tst_guitests.cpp)
add_test_executable(FailedLogin tst_failed_login.cpp)

add_test(NAME InteractionTests COMMAND InteractionTests)
add_test(NAME FailedLogin COMMAND FailedLogin)

target_compile_definitions(FailedLogin PRIVATE QT_QML_DEBUG)

