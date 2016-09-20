#include <nytl/nytl.hpp>
#include <nytl/triangle.hpp>
#include <nytl/line.hpp>
#include <nytl/transform.hpp>

#include <glm/glm.hpp>
#include <nytl/integration/glm.hpp>

#include <osg/Matrixf>
#include <osg/Matrixd>

#include <osg/Vec2f>
#include <osg/Vec2d>
#include <osg/Vec2i>
#include <osg/Vec2ui>
#include <osg/Vec2b>
#include <osg/Vec2ub>
#include <osg/Vec2s>
#include <osg/Vec2us>

#include <osg/Vec3f>
#include <osg/Vec3d>
#include <osg/Vec3i>
#include <osg/Vec3ui>
#include <osg/Vec3b>
#include <osg/Vec3ub>
#include <osg/Vec3s>
#include <osg/Vec3us>

#include <osg/Vec4f>
#include <osg/Vec4d>
#include <osg/Vec4i>
#include <osg/Vec4ui>
#include <osg/Vec4b>
#include <osg/Vec4ub>
#include <osg/Vec4s>
#include <osg/Vec4us>
#include <nytl/integration/osg.hpp>

#include <iostream>
#include <fstream>
#include <cassert>
#include <set>
using namespace nytl;

struct Test {};

int main()
{
	//glm
	static_assert(std::is_same<decltype(fromGlm(glm::vec4{})), nytl::Vec4f>::value, "");
	static_assert(std::is_same<decltype(fromGlm(glm::mat4x4{})), nytl::Mat4f>::value, "");
	static_assert(std::is_same<decltype(fromGlm(glm::mat4x2{})), nytl::Mat24f>::value, "");

	static_assert(std::is_same<decltype(toGlm(nytl::Vec4f{})), glm::vec4>::value, "");
	static_assert(std::is_same<decltype(toGlm(nytl::Mat4f{})), glm::mat4x4>::value, "");
	static_assert(std::is_same<decltype(toGlm(nytl::Mat24f{})), glm::mat4x2>::value, "");

	nytl::Vec4f vec1 = nytl::convert(glm::vec4{});
	nytl::unused(vec1);

	//osg
	static_assert(std::is_same<decltype(fromOsg(osg::Vec4f{})), nytl::Vec4f>::value, "");
	static_assert(std::is_same<decltype(fromOsg(osg::Matrixd{})), nytl::Mat4d>::value, "");

	static_assert(std::is_same<decltype(toOsg(nytl::Vec4f{})), osg::Vec4f>::value, "");
	static_assert(std::is_same<decltype(toOsg(nytl::Mat4d{})), osg::Matrixd>::value, "");

	nytl::Vec3c vec2 = nytl::convert(osg::Vec3b{});
	nytl::unused(vec2);
}
