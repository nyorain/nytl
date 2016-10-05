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
