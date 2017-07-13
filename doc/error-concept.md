Alternative error handling version:
Give the user the possibility to handle runtime errors.
Note that the default error function must not be in assure (since there are no header guards).
Not used because probably out of scope for nytl, makes things more complicated.

```cpp
/// Define this macro and implement a custom nytl::runtimeErrorHandler that
/// should be called when a runtime error occurs.
#ifndef NYTL_CUSTOM_RUNTIME_ERROR
namespace nytl {
	void runtimeErrorHandler(const char* func, const char* expr, const char* msg) {
		std::string message = "nytl runtime error in '";
		message += func;
		message += "': ";
		message += msg;
		message += " (";
		message += expr;
		message += ")";
		throw std::invalid_argument(message); \
	}
}
#endif

/// Utility macro that makes sure expr is fulfilled.
/// Will test it at compile time if static_ is true.
#define nytl_assure(static_, expr, message) \
	if constexpr(static_) { \
		static_assert(expr, message); \
	} else if constexpr(NYTL_RUNTIME_CHECK) { \
		if(!(expr)) \
			::nytl::runtimeErrorHandler(__FUNCTION__, #expr, message); \
	}
```
