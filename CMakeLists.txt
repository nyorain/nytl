CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)
PROJECT(nytl)

set(vmajor 0)
set(vminor 2)
set(vpatch 0)

set(CMAKE_BUILD_TYPE Debug)
option(Test "Build and execute the nytl tests" OFF)

include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -Wall -Wextra") # C++ 17

if(Test)
	add_subdirectory(doc/tests)
	enable_testing()
	add_test(NAME utilityTest COMMAND test_utility)
	add_test(NAME mathTest COMMAND test_math)
	add_test(NAME funcTest COMMAND test_func)
endif()

#pkg-config
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/nytl.pc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/nytl.pc"
	@ONLY)

# install: pkg-config and nytl directory
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/nytl.pc" DESTINATION lib/pkgconfig)
install(DIRECTORY nytl DESTINATION include)
