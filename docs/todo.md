TODO for nytl
=============

- integration
- rework (?)
	- line/triangle/tetrahedron (with ops)
	- misc (printVars, real memberCallback)
	- transform (rework!)
- rect improvements (specializations?)
	- rectOps
- refVec (?)
- more tests
- simplexOps
	- barycentric <-> world space
	- some more work... specializations?
- more/better vecOps/matrixOps
	- submatrix and subvector functions
	- more lu decomp algorithms (like e.g. crout)
	- various matrix checks: symmetric? (positive/negative/in-) definite?
- try simd/sse

- ? (to be potentially reintroduced (with rework) later on)
	- system
	- tmp (more tuple ops, integer sequence ops)
	- cache
	- compFunc (more descriptive name would be good if possible)

- make StringParam template for char type (like std::string is? make e.g. utf32 possible)

- C++17
	- matOps/vecOps: structured bindings
	- scope: remove make* wrapper functions
	- connection: remove make* wrapper functions
	- tmpUtil: remove void_t (also remove nytl::Expand?)
		- look for further duplicates
