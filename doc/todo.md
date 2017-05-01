TODO for nytl
=============

- testing !important
	- split utility testing
	- vec: test dynVec, test cast operator, test __all__ operators
	- practical field tests for already unit tested interfaces
		- optimize performance (if possible) (?)
- move Approx/approx to nytl (nytl/approx.hpp)
- is `int a = nytl::convert(1.f)` really a good idea? i guess not somehow...
- check correct header includes (nytl, stl)
- vecOps: compile time error for complex numbers in non-supporting operations (?)
	- e.g. dot&angle: they might not behave as expected
- vec rework (see associated branch) [C++17]
	- make operators even more explicit, e.g. dont allow vec2 + vec3!
- integration (glm, eigen)
- rework (?)
	- line/triangle/tetrahedron (with ops)
	- misc (printVars, real memberCallback)
	- transform (rework!)
- rect improvements
	- further rectOps
	- rectOps tests!
- refVec (?)
	- would be hard [in the sense of non-trivial] to implement
		- custom iterators, could not provide data() function
	- would be very convenient to e.g. access a column of a matrix
- more/better vecOps/matrixOps
	- submatrix and subvector functions (possible as generic? otherwise only for Vec/Mat)
	- more lu decomp algorithms (like e.g. crout)
	- various matrix checks: (positive/negative/in-) definite?
- try simd/sse
- ovservable connection

- ? (to be potentially reintroduced (with rework) later on)
	- system
	- tmp (more tuple ops, integer sequence ops)
	- cache
	- compFunc (more descriptive name would be good if possible)
	- simplexOps (see commit afd548957748a6218f1d4c4ae77abba533c34809)
		- pretty bad (ops only working for square matrices!)
		- barycentric <-> world space
		- some more work... specializations?

- make StringParam template for char type (like std::string is? make e.g. utf32 possible)
- noexcept fixes
- seperation interface/implemetation where neeeded (callback/connection/typemap)

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
