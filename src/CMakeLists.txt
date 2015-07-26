#fpic
set (CMAKE_POSITION_INDEPENDENT_CODE TRUE)

find_package(DL REQUIRED)

add_library(nyutil SHARED
	thread.cpp 
	misc.cpp 
	argParser.cpp
	module.cpp)
target_compile_features(nyutil PRIVATE cxx_range_for)

include_directories(
	${DL_INCLUDE_DIR}	
)

target_link_libraries(nyutil
	${DL_LIBRARY}
)

install(DIRECTORY nyutil DESTINATION include/nyutil)
install(TARGETS nyutil DESTINATION lib)
