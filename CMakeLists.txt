CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(project_nyutil)

set(NYUTIL_VMAJOR 0)
set(NYUTIL_VMINOR 1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake)

include_directories(src)
add_subdirectory(src)

#pkg-config
configure_file( 
	"${CMAKE_CURRENT_SOURCE_DIR}/nyutil.pc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/nyutil.pc"
	@ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/nyutil.pc" DESTINATION lib/pkgconfig)
