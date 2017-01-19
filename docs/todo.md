TODO for nytl
=============

- integration (glm, eigen)
- rework (?)
	- line/triangle/tetrahedron (with ops)
	- misc (printVars, real memberCallback)
	- transform (rework!)
- rect improvements
	- further rectOps
	- rectOps tests!
- refVec (?)
- more tests
- simplexOps
	- barycentric <-> world space
	- some more work... specializations?
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

- make StringParam template for char type (like std::string is? make e.g. utf32 possible)
- noexcept fixes
- seperation interface/implemetation where neeeded (callback/connection/typemap)

- C++17
	- matOps/vecOps: structured bindings
	- scope: remove make* wrapper functions
	- connection: remove make* wrapper functions
	- tmpUtil: remove void_t (also remove nytl::Expand?)
		- look for further duplicates
	- easier isCallable (using FunctionTraits & constexpr if)
