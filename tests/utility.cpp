#include <nytl/nytl.hpp>
using namespace nytl;
using namespace nytl::enumOps;

enum class enumType
{
	entry1,
	entry2,
	entry3,
	entry4
};

int main()
{
	//enumOPs
	{
		auto e = entry1 | entry2 & ~entry4;
		static_assert(std::is_same<decltype(e), enumType>::value, "");

		e |= e & ~entry3;
		e ^= entry4;
	}


}
