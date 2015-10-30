CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
PROJECT(nytl)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake)

include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(tests)

find_package(Doxygen)
if(DOXYGEN_FOUND)
	add_custom_target(doc 
		${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/doc/doxygen/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)

#pkg-config
configure_file( 
	"${CMAKE_CURRENT_SOURCE_DIR}/nytl.pc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/nytl.pc"
	@ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/nytl.pc" DESTINATION lib/pkgconfig)
install(DIRECTORY nytl DESTINATION include)

#tests
enable_testing()
add_test(NAME utilityTest COMMAND test_utility)
add_test(NAME mathTest COMMAND test_math)
add_test(NAME funcTest COMMAND test_func)
