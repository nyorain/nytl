CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(project_nyutil)

set(NYUTIL_VMAJOR 0)
set(NYUTIL_VMINOR 1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake)

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
#configure_file( 
#	"${CMAKE_CURRENT_SOURCE_DIR}/nyutil.pc.in"
#	"${CMAKE_CURRENT_BINARY_DIR}/nyutil.pc"
#	@ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/nyutil.pc" DESTINATION lib/pkgconfig)
install(DIRECTORY nyutil DESTINATION include)
