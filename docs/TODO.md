TODO for nytl
=============

- fwd
- integration
- tests
- rework (?)
	- line/triangle/tetrahedron (with ops)
	- misc (printVars, real memberCallback)
	- system
	- tmp
	- transform (rework!)
- typemap (overhaul)
- rect
	- include rectRegion? not really needed...
	- rectOps
- refVec (?)
- simplexOps
	- barycentric <-> world space
- more/better vecOps/matrixOps
	- more lu decomp algorithms (like e.g. crout)
	- various matrix checks: symmetric? (positive/negative/in-) definite?
- try simd/sse

- ? (to be potentially reintroduced (with rework) later on)
	- cache
	- functionTraits
	- compFunc (more descriptive name would be good if possible)

- C++17
	- matOps/vecOps: structured bindings
	- scope: remove make* Wrapper functions
	- tmpUtil: remove void_t (also remove nytl::Expand?)
		- look for further duplicates
