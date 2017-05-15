add_executable(vec vec.cpp)
add_test(vec vec)

add_executable(dynVec dynVec.cpp)
add_test(dynVec dynVec)

add_executable(mat mat.cpp)
add_test(mat mat)

add_executable(typemap typemap.cpp)
add_test(typemap typemap)

add_executable(referenced referenced.cpp)
add_test(referenced referenced)

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

add_executable(connection connection.cpp)
add_test(connection connection)

add_executable(approx approx.cpp)
add_test(approx approx)

add_executable(utf utf.cpp)
add_test(utf utf)

add_executable(stringParam stringParam.cpp)
add_test(stringParam stringParam)

add_executable(flags flags.cpp)
add_test(flags flags)

add_executable(convert convert.cpp)
add_test(convert convert)

# compile-time tests only
add_executable(nonCopyable nonCopyable.cpp)
add_executable(tmp tmp.cpp)
add_executable(functionTraits functionTraits.cpp)
