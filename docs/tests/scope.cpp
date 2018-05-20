#include "test.hpp"
#include <nytl/scope.hpp>

TEST(general) {
	auto i = 0u;

	{
		auto guard1 = nytl::ScopeGuard {[&]{
			++i;
			throw std::runtime_error("<This error is expected>");
		}};

		auto guard2 = nytl::ScopeGuard {[&]{
			++i;
		}};
	}

	EXPECT(i, 2u);
}

TEST(success) {
	auto i = 0u;

	try {
		auto guard = nytl::SuccessGuard {[&]{
			EXPECT(true, false);
			++i;
		}};

		throw 42;
	} catch(...) {}

	EXPECT(i, 0u);
	try {
		auto guard = nytl::SuccessGuard {[&]{
			++i;
		}};
	} catch(...) {}

	EXPECT(i, 1u);
}

TEST(exception) {
	auto i = 0u;

	try {
		auto guard = nytl::ExceptionGuard {[&]{
			EXPECT(true, false);
			++i;
		}};
	} catch(...) {}

	EXPECT(i, 0u);
	try {
		auto guard = nytl::ExceptionGuard {[&]{
			++i;
		}};
		throw 42;
	} catch(...) {}

	EXPECT(i, 1u);
}

TEST(nested) {
	auto i = 0u;

	try {
		auto guard = nytl::ScopeGuard{[&]{
			auto guard = nytl::ScopeGuard{[&]{
				i += 1u;
			}};

			auto eguard = nytl::ExceptionGuard{[&]{
				i += 10u;
			}};

			auto sguard = nytl::SuccessGuard{[&]{
				i += 100u;
				EXPECT(true, false);
			}};

			throw std::runtime_error("<Again an expected error>");
		}};

		throw std::runtime_error("<Another expected error>");
	} catch(...) {
	}

	EXPECT(i, 11u);
}
