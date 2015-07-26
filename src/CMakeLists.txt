add_library(nyutil STATIC 
	thread.cpp 
	misc.cpp 
	argParser.cpp
	module.cpp)
target_compile_features(nyutil PRIVATE cxx_range_for)

install(DIRECORIES nyutil DESTINATION include/nyutil)
install(TARGETS nyutil DESTINATION lib)
