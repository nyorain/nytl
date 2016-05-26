#include <nytl/nytl.hpp>
using namespace nytl;

enum class enumType
{
	entry1,
	entry2,
	entry3,
	entry4
};

NYTL_ENABLE_ENUM_OPS(enumType)

int main()
{
	//enumOPs
	{
		auto e = (enumType::entry1 | enumType::entry2) & enumType::entry4;
		e |= e & enumType::entry3;
		e ^= enumType::entry4;
	}


}
