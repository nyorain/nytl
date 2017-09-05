tvec = executable('vec', 'vec.cpp', dependencies: nytl_dep)
test('vec', tvec)

tdynVec = executable('dynVec', 'dynVec.cpp', dependencies: nytl_dep)
test('dynVec', tdynVec)

tmat = executable('mat',  'mat.cpp', dependencies: nytl_dep)
test('mat', tmat)

ttypemap = executable('typemap', 'typemap.cpp', dependencies: nytl_dep)
test('typemap', ttypemap)

treferenced = executable('referenced', 'referenced.cpp', dependencies: nytl_dep)
test('referenced', treferenced)

tcallback = executable('callback', 'callback.cpp', dependencies: nytl_dep)
test('callback', tcallback)

tclone = executable('clone', 'clone.cpp', dependencies: nytl_dep)
test('clone', tclone)

tobserve = executable('observe', 'observe.cpp', dependencies: nytl_dep)
test('observe', tobserve)

tspan = executable('span', 'span.cpp', dependencies: nytl_dep)
test('span', tspan)

trect = executable('rect', 'rect.cpp', dependencies: nytl_dep)
test('rect', trect)

tconnection = executable('connection', 'connection.cpp', dependencies: nytl_dep)
test('connection', tconnection)

tapprox = executable('approx', 'approx.cpp', dependencies: nytl_dep)
test('approx', tapprox)

tutf = executable('utf', 'utf.cpp', dependencies: nytl_dep)
test('utf', tutf)

tflags = executable('flags', 'flags.cpp', dependencies: nytl_dep)
test('flags', tflags)

tconvert = executable('convert', 'convert.cpp', dependencies: nytl_dep)
test('convert', tconvert)

# compile-time tests only
executable('nonCopyable', 'nonCopyable.cpp', dependencies: nytl_dep)
executable('tmp', 'tmp.cpp', dependencies: nytl_dep)
executable('functionTraits', 'functionTraits.cpp', dependencies: nytl_dep)
