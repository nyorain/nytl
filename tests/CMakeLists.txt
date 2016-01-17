add_executable(test_func func.cpp)
set_property(TARGET test_func PROPERTY CXX_STANDARD 11)

add_executable(test_math math.cpp)
set_property(TARGET test_math PROPERTY CXX_STANDARD 11)

add_executable(test_utility utility.cpp)
set_property(TARGET test_utility PROPERTY CXX_STANDARD 11)

#private test for new features
add_executable(test_private test.cpp)
set_property(TARGET test_private PROPERTY CXX_STANDARD 11)
