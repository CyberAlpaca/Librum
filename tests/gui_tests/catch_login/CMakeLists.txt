cmake_minimum_required(VERSION 3.16)

project(CatchLibrumTests LANGUAGES CXX)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Gui Quick Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Qml Quick Test Concurrent)
find_package(Qt6 REQUIRED COMPONENTS QuickControls2)

# Link test executable against Catch2
add_executable(tests_example test_example.cpp test_main.cpp)
target_link_libraries(tests_example PRIVATE Catch2::Catch2 Qt6::Core Qt6::Quick suri)

