tvec = executable('vec', 'vec.cpp', dependencies: nytl_dep)
test('vec', tvec)

tmat = executable('mat',  'mat.cpp', dependencies: nytl_dep)
test('mat', tmat)

tcallback = executable('callback', 'callback.cpp', dependencies: nytl_dep)
test('callback', tcallback)

trcallback = executable('rcallback', 'rcallback.cpp', dependencies: nytl_dep)
test('rcallback', trcallback)

tclone = executable('clone', 'clone.cpp', dependencies: nytl_dep)
test('clone', tclone)

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

tscope = executable('scope', 'scope.cpp', dependencies: nytl_dep)
test('scope', tscope)

# compile-time tests only
executable('tmp', 'tmp.cpp', dependencies: nytl_dep)
executable('functionTraits', 'functionTraits.cpp', dependencies: nytl_dep)
