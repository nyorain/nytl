add_executable(test_func func.cpp)
target_compile_features(test_func PRIVATE cxx_range_for)

add_executable(test_math math.cpp)
target_compile_features(test_math PRIVATE cxx_range_for)

add_executable(test_utility utility.cpp)
target_compile_features(test_utility PRIVATE cxx_range_for)

#private test for new features
add_executable(test_private test.cpp)
target_compile_features(test_private PRIVATE cxx_range_for)
