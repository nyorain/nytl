// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <cstdint>
#include <utility>

//The goal:
//make it possible to construct a vector like this:
//auto v = nytl::Vec<7, int>(vec2, vec3, 5, vec2);

using Expand = int[];

//replace the next two with specializable structs.
//or are functions sufficient?
template<typename T>
constexpr std::size_t componentSize() { return 0; }

template<std::size_t I, typename T>
constexpr auto getComponent(T&& t) { return t[I]; }

template<std::size_t N, typename Head, typename... Tail>
auto constexpr selectComponent(Head&& head, Tail&&... tail)
{
	constexpr auto size = componentSize<Head>();
	if(size > N) return getComponent<N>(head);
	return selectComponent<N - size>();
}

template<typename T>
struct ComponentGetter
{

};

// template<std::size_t D, typename T, typename = std::make_index_sequence<D>> struct InitVec;
// template<std::size_t D, typename T, std::size_t... I>
// struct InitVec<D, T, std::index_sequence<I...>>
// {
// 	template<typename... Args>
// 	static constexpr void call(Vec<D, T>& vec, Args&&... args)
// 	{
//		using Seq = std::make_index_sequence<sizeof...(Args)>;
// 		vec[I] = Component<I, Args...>::call(args...);
// 	}
// };
