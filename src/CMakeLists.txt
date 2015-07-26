add_library(nyutil STATIC 
	thread.cpp 
	misc.cpp 
	argParser.cpp
	module.cpp)
target_compile_features(nyutil PRIVATE cxx_range_for)

if (LINUX)
     set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fPIC")
endif()

install(DIRECTORY nyutil DESTINATION include/nyutil)
install(TARGETS nyutil DESTINATION lib)
