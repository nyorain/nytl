#include "bugged.hpp"
#include <nytl/flags.hpp>

// TODO: static assert invalid expressions (i.e. for enums without ops enabled)

enum class Enum {
	entry0 = 0,
	entry1 = 1,
	entry2 = 2,
	entry3 = 4
};

NYTL_FLAG_OPS(Enum)

TEST(flags) {
	constexpr auto entry23 = Enum::entry2 | Enum::entry3;
	static_assert(entry23.value() == 6, "flags test #1");

	constexpr auto entry3 = entry23 & Enum::entry3;
	static_assert(entry3.value() == 4, "flags test #2");

	constexpr auto entryNot3 = ~Enum::entry3;
	static_assert((entryNot3 & Enum::entry0) == Enum::entry0, "flags test #3");
	static_assert((entryNot3 & Enum::entry1) == Enum::entry1, "flags test #4");
	static_assert((entryNot3 & Enum::entry2) == Enum::entry2, "flags test #5");

	static_assert(static_cast<unsigned int>(entry23) == 6, "flags cast test 1");
	static_assert(static_cast<int>(entry3) == 4, "flags cast test 1");
}
