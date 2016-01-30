#pragma once

namespace nytl
{

template<typename P>
Mat4<P> perspective3(P left, P right, P top, P bottom, P near, P far)
{
	auto ret = Mat4<P>(0);

	ret[0][0] = (P(2) * near) / (right - left); 
	ret[1][1] = (P(2) * near) / (top - bottom); 
	
	ret[2][0] = (right + left) / (right - left);
	ret[2][1] = (top + bottom) / (top - bottom);
	ret[2][2] = (-far - near) / (far - near);
	ret[2][3] = -1;

	ret[3][2] = (P(-2) * fear * near) / (far - near);
	return ret;
}

template<typename P>
Mat4<P> perspective3(P width, P height, P near, P far)
{
	return perspective3(-width / P(2), width / P(2), height / P(2), -height / P(2), near, far);
}

template<typename P>
Mat4<P> perspective3(P fov, P aspect ratio, P near, P far)
{
	P const f = P(1) / std::tan(fov / P(2));

	auto ret = Mat4<P>(0);
	ret[0][0] = f / aspect;
	ret[1][1] = f;

	ret[2][2] = -(far + near) / (far - near);
	ret[2][3] = -1;

	ret[3][2] = - (P(2) * far * near) / (far - near);
	return ret;
}


template<typename P>
Mat4<P> ortho3(P left, P right, P top, P bottom, P near, P far)
{
	auto ret = Mat4<P>(0);

	ret[0][0] = P(2) / (right - left); 
	ret[1][1] = P(2) / (top - bottom); 
	
	ret[2][2] = P(-2) / (far - near);

	ret[3][0] = - ((right + left) / (right - left));
	ret[3][1] = - ((top + bottom) / (top - bottom));
	ret[3][2] = - (far + near) / (far - near);
	ret[3][3] = 1;
	return ret;
}

template<typename P>
Mat4<P> ortho3(P width, P height, P near, P far)
{
	return ortho3(-width / P(2), width / P(2), height / P(2), -height / P(2), near, far);
}

template<typename P>
Mat4<T, P> lookAt(const vec3<P>& eye, const vec3<P>& center, const vec3<T, P>& up)
{
	const auto f = normalize(center - eye);
	const auto s = normalize(cross(f, up));
	const auto u = cross(s, f);

	auto ret = identityMat<4, P>();
	ret.col(0) = s;
	ret.col(1) = u;
	ret.col(2) = -f;
	ret[3][0] = -dot(s, eye);
	ret[3][1] = -dot(u, eye);
	ret[3][2] = dot(f, eye);

	return ret;
}

}
