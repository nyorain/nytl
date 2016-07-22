CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)
PROJECT(nytl)

set(NYTL_VMAJOR 0)
set(NYTL_VMINOR 2)
set(NYTL_VPATCH 0)

set(CMAKE_BUILD_TYPE Debug)
option(NYTL_TEST "Build the nytl tests" ON)

include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

if(NYTL_TEST)
	add_subdirectory(tests)
	enable_testing()
	add_test(NAME utilityTest COMMAND test_utility)
	add_test(NAME mathTest COMMAND test_math)
	add_test(NAME funcTest COMMAND test_func)
endif(NYTL_TEST)

find_package(Doxygen)
if(DOXYGEN_FOUND)
	add_subdirectory(doc)
endif(DOXYGEN_FOUND)

#pkg-config
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/nytl.pc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/nytl.pc"
	@ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/nytl.pc" DESTINATION lib/pkgconfig)
install(DIRECTORY nytl DESTINATION include)
