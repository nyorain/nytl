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
		auto e = (enumType::entry1 | enumType::entry2) & ~enumType::entry4;
		static_assert(std::is_same<decltype(e), enumType>::value, "");

		e |= e & ~enumType::entry3;
		e ^= enumType::entry4;
	}


}
