cmake_minimum_required(VERSION 2.8)
project(nytl)

set(vmajor 0)
set(vminor 4)
set(vpatch 0)

option(Test "Build the nytl tests" off)
include_directories(${CMAKE_SOURCE_DIR})

# it currently only compiles on gcc/clang anyways
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-braces -Wno-unused-const-variable")

set(CMAKE_BUILD_TYPE Debug)

if(Test)
	enable_testing() # must be called here
	add_subdirectory(doc/tests)
endif()

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/nytl.pc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/nytl.pc"
	@ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/nytl.pc" DESTINATION lib/pkgconfig)
install(DIRECTORY nytl DESTINATION include)
