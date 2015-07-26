add_library(nyutil STATIC 
	thread.cpp 
	misc.cpp 
	argParser.cpp
	module.cpp)
target_compile_features(nyutil PRIVATE cxx_range_for)

install(DIRECTORY nyutil DESTINATION include/nyutil)
install(LIBRARY nyutil DESTINATION lib)
