cmake_minimum_required(VERSION 3.5)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)


project(test_slide1)

find_package(OpenCV REQUIRED )

file(GLOB sources "./*.cpp")

add_executable(test_slide1 ${sources})

target_include_directories(test_slide1 PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(test_slide1 PRIVATE "/usr/local/include/openslide")

target_link_libraries(test_slide1 ${OpenCV_LIBS})
target_link_libraries(test_slide1 "/usr/local/lib/libopenslide.so")

