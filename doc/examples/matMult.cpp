#include <nytl/mat.hpp>
#include <stdio.h>

// simple mat mult example
// useful to test compiler vectorization

extern nytl::Mat4f a;
extern nytl::Mat4f b;

alignas(16) nytl::Mat4f a = {
	1, 2, 3, 4,
	4, 3, 2, 1,
	0, 1, 0, 1,
	7, 6, 5, 2
};

alignas(16) nytl::Mat4f b = {
	2, 2, 7, 4,
	0, 3, 0, 1,
	1, 4, 2, 2,
	0, 2, 7, 2
};

int main()
{
	auto ret = a * b;
	for(auto r = 0u; r < ret.rows(); ++r)
		for(auto c = 0u; c < ret.cols(); ++c)
			printf("%f\n", ret[r][c]);
}
