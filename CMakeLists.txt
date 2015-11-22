CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
PROJECT(nytl)

set(NYTL_VMAJOR 0)
set(NYTL_VMINOR 1)
option(NYTL_BUILD_TESTS OFF)

include_directories(${CMAKE_SOURCE_DIR})

if(NYTL_BUILD_TESTS)
	add_subdirectory(tests)
	enable_testing()
	add_test(NAME utilityTest COMMAND test_utility)
	add_test(NAME mathTest COMMAND test_math)
	add_test(NAME funcTest COMMAND test_func)
endif(NYTL_BUILD_TESTS)

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
