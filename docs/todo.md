TODO for nytl
=============

- testing !important
	- left completely untested: template tuple (operations)
	- improve: scope, more vec/vecOps testing needed
	- fix todos at the top of unit tests
	- vec: test more of/with dynVec, test cast operator, test __all__ operators
	- test more rectOps/convert/flags (basic ops)
	- test more matrix operations
		- simplify them?
	- practical field tests for already unit tested interfaces
		- optimize performance (if possible) (?)
- is `int a = nytl::convert(1.f)` really a good idea? not sure... [AutoCastable]
- check correct header includes (nytl itself, stl)
- vecOps cw/ip
	- test!
	- for scalar functions
- integration (glm, eigen)
- rework the following files (and readd them) (?)
	- line/triangle/tetrahedron (with ops)
	- misc (printVars, real memberCallback)
	- transform (rework!)
		- not too generic this time, it is ok
		- matrix operation probably enough, no need for a transform class
- rect improvements
	- further rectOps
	- rectOps tests!
- more/better vecOps/matrixOps
	- submatrix and subvector functions (possible as generic? otherwise only for Vec/Mat)
	- more lu decomp algorithms (like e.g. crout)
	- various matrix checks: (positive/negative/in-) definite?
- fix callback connection id spec, see callback.md

Not to important
===============

- refVec (?)
	- would be hard [non-trivial] to implement
		- custom iterators, could not provide data() function
	- would be very convenient to e.g. access a column of a matrix
- try simd/sse (?)
	- is there really any speed advantage in it? profile (and look at the assembler of) examples
	- or rather write code in a way that allows the compiler to optimize it
- noexcept fixes
- remove macros from scope.hpp? nice since they give automatic name but not really used
- seperation interface/implemetation where neeeded (callback/connection/typemap)
- vecOps: compile time error for complex numbers in non-supporting operations (?)
	- e.g. dot&angle: they might not behave as expected
	- how to go about non-number types anyways?
		- reintroduce nytl::FieldTraits?
			- or go already for nytl::VectorTraits?
- ? (to be potentially reintroduced (with rework) later on)
	- system
	- tmp (more tuple ops, integer sequence ops)
	- cache
	- compFunc (more descriptive name would be good if possible)
	- simplexOps (see commit afd548957748a6218f1d4c4ae77abba533c34809)
		- pretty bad (ops only working for square matrices!)
		- barycentric <-> world space
		- some more work... specializations?


- C++17
	- merge branch
		- remove nytl/any.hpp
	- matOps/vecOps: use structured bindings
		- especially lu[p] mess
	- scope: remove make* wrapper functions
	- connection: remove make* wrapper functions
	- tmpUtil: remove void_t (also remove nytl::Expand?)
		- look for further duplicates
	- easier isCallable (using FunctionTraits & constexpr if)
		- possible?
