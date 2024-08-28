# Language
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

add_subdirectory(libs/suri/)
add_subdirectory(libs/Catch2/)
add_subdirectory(demo_qtest_tests/)
add_subdirectory(demo_catch2_tests/)
