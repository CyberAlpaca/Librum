cmake_minimum_required(VERSION 3.16)

project(CatchLibrumTests LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Gui Quick Test Qml QuickControls2 Concurrent Widgets)

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
    Qt6::Core
    Qt6::Concurrent
    Qt6::Quick
    Qt6::Qml
    Qt6::Widgets
    Qt6::Test
    Qt6::QuickControls2
    ${MUPDF_OUTPUT}
    suri
    Catch2::Catch2WithMain
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

# Define the function to create a test executable
function(create_test TARGET_NAME SOURCE_FILES)
    add_executable(${TARGET_NAME} ${SOURCE_FILES} TestFixture.cpp ${COMMON_EXE_SOURCES})
    target_sources(${TARGET_NAME} PRIVATE macros.h)
    target_include_directories(${TARGET_NAME} PRIVATE ${COMMON_INCLUDE_DIRS})
    target_link_libraries(${TARGET_NAME} PRIVATE ${COMMON_PRV_LIBRARIES})
    target_link_libraries(${TARGET_NAME} PUBLIC ${COMMON_PUBLIC_LIBRARIES})
endfunction()

# Add tests
create_test(test_example "test_example.cpp")
create_test(tests_invalid_credentials "test_invalid_credentials.cpp")
create_test(tests_settings "test_settings.cpp")



