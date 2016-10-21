Vec operations
==============

Operations are always only performed on subvecs that have a common size.
The not used components will be left untouched. This might look weird first for operations
as multiplication or division but allows easy operations for kinds of vectors.

Some examples:

Vec3i(1, 2, 3) * Vec3i(2, 3, 4) = Vec3i(2, 6, 12)
Vec3i(1, 2, 3) * Vec3f(0.5f, 2.f, 1 / 3.f) = Vec3f(0.5f, 4.f, 1.f)

Vec3i(1, 2, 3) + Vec2i(1, 2) = Vec3i(2, 4, 3)
Vec2i(1, 2) * Vec3i(2, 3, 4) = Vec3i(2, 4, 4)
Vec3f(1.f, 2.f, 5.f) / Vec2i(2, 4) = Vec3f(0.5f, 0.5f 5.f)

5 - Vec2i(4, 5) = Vec2i(1, 0)
Vec4ui(0, 0, 1, 0) + 22 = Vec4ui(22, 22, 23, 22)

Vec2ui(1, 2) > Vec4ui(4, 5, 6, 7) = Vec2b(false, false)

CompFunc
========

Problems with types that can be constructed from any object like std::any.

Signature: void(int, int)
Function: void(std::any)
	=> The Function will be called with the first int parameter wrapped inside an any object

Signature: void(int, std::any)
Function: void(std::any)
	=> The Function will be called with the first int parameter wrapped inside an any object

The solution to this problem is nytl::IsCompatible or nytl::ConversionException.
Those templates can be specialized to make conversion not possible for CompFunc objects.

The nytl headers do not provide such a specialization for std::any (since it can be really useful)
but users of nytl may do so. The only thing nytl specialized is that nytl::ConnectionRef will
never be converted to sth else (like e.g. wrapped inside an any object), which is useful for 
nytl::Callback.

One additionally have to care about type conversions:

Signature: void(int, const std::any&, int)
Function: void(int, std::any&&)
	=> The Function will be called with the first and last parameter, the last one wrapped into
		an any object. The second one (the const std::any&) will not be used, since it cannot
		be converted to a std::any&& object.

Callback
========

auto id = callback.add(&func);
callback.remove(id);

callback.add(&func);

auto connection = nytl::ConnectionGuard(callback, callback.add(&func));
