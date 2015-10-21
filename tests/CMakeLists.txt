add_executable(test_func func.cpp)
target_compile_features(test_func PRIVATE cxx_range_for)

add_executable(test_math func.cpp)
target_compile_features(test_math PRIVATE cxx_range_for)

add_executable(test_utility func.cpp)
target_compile_features(test_utility PRIVATE cxx_range_for)
