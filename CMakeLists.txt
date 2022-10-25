cmake_minimum_required(VERSION 3.5)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
set(CMAKE_CXX_STANDARD 17)  


project(test_slide2)


file(GLOB source1 "./src/*.cpp")
set(source2 
../ds/kb_openslide.cpp ../ds/kb_slide_info.cpp 
../xml/kb_xml.cpp ../xml/kb_xml_io.cpp 
)
add_executable(test_slide2 ${source1} ${source2})

include_directories("${CMAKE_CURRENT_LIST_DIR}/../common")
include_directories("${CMAKE_CURRENT_LIST_DIR}/../cv")
include_directories("${CMAKE_CURRENT_LIST_DIR}/../ds")
include_directories("${CMAKE_CURRENT_LIST_DIR}/../xml")


target_link_libraries(test_slide2 "${CMAKE_CURRENT_LIST_DIR}/../ds/libopenslide.lib")

