add_executable(vec vec.cpp)
add_test(vec vec)

add_executable(mat mat.cpp)
add_test(mat mat)

add_executable(typemap typemap.cpp)
add_test(typemap typemap)

add_executable(utility utility.cpp)
add_test(utility utility)

add_executable(callback callback.cpp)
add_test(callback callback)

add_executable(clone clone.cpp)
add_test(clone clone)

add_executable(observe observe.cpp)
add_test(observe observe)

add_executable(span span.cpp)
add_test(span span)

add_executable(rect rect.cpp)
add_test(rect rect)

# nonCopyable is compile-time test only
add_executable(nonCopyable nonCopyable.cpp)
