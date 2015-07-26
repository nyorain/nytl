#fpic
set (CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_library(nyutil SHARED
	thread.cpp 
	misc.cpp 
	argParser.cpp
	module.cpp)
target_compile_features(nyutil PRIVATE cxx_range_for)

install(DIRECTORY nyutil DESTINATION include/nyutil)
install(TARGETS nyutil DESTINATION lib)
