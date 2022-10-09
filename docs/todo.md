TODO for nytl
=============

- more testing !important
	- improve: more vec/vecOps, rectOps testing needed
	- fix todos at the top of unit tests
	- test more rectOps/flags (basic ops)
	- utf stress test
		- https://stackoverflow.com/questions/1319022/really-good-bad-utf-8-example-test-data
	- test more matrix operations
		- simplify them?
	- practical field tests for already unit tested interfaces
		- optimize performance (if possible) (?)

Not too important
===============

- add more rectOps and vecOps (port utility from other projects)
- Vec specilizations stl-like utility functions
- Vec4 specialization? Use x,y,z,w?
- more/better vecOps/matrixOps
	- submatrix and subvector functions (possible as generic? otherwise only for Vec/Mat)
	- more lu decomp algorithms (like e.g. crout)
		- non-square lu decomp?
	- various matrix checks: (positive/negative/in-) definite?
- noexcept fixes
- seperation interface/implemetation where neeeded (callback/connection/typemap)
- ? (to be potentially reintroduced (with rework) later on)
	- system
	- tmp (more tuple ops, integer sequence ops)
	- cache
	- compFunc (more descriptive name would be good if possible)
	- simplexOps (see commit afd548957748a6218f1d4c4ae77abba533c34809)
		- pretty bad (ops only working for square matrices!)
		- barycentric <-> world space
		- some more work... specializations?
- easier isCallable (using FunctionTraits & constexpr if)
	- possible?
- think about nytl/convert (checkout commit fa1c07ba599e2adc590521d981243f290754f9f5)
	- is `int a = nytl::convert(1.f)` really a good idea? not sure... [AutoCastable]
	- arrayCast was probably a good idea (use it in simplex cast operator then again)
- integration (glm, eigen)
- rework the following files (and readd them) (?)
	- line/triangle/tetrahedron (with ops)
	- misc (printVars, real memberCallback)
	- transform (rework!)
		- not too generic this time, it is ok
		- matrix operation probably enough, no need for a transform class
	- tuple (see commit fa1c07ba599e2adc590521d981243f290754f9f5)
- rename validExpression as it is named in the stl
- extend/improve vecOps cw/ip
	- test!
	- for scalar functions
- Callback: add possibility for called functions to remove themselves.
  Should be doable without significant overhead (optionally pass additional
  parameter).
  `Connection addSelfRemoving(std::function<Ret(bool& removeSelf, Args...)>)`
- Make UniqueConnection unique_ptr?
