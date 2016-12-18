#pragma once

#include <cstdint>
#include <type_traits>
#include <iterator>
#include <utility>
#include <functional>
#include <vector>

// tmp
#include <iostream>
#include <cassert>

template<typename A, typename> using VariadicDummy = A;

template<typename T, std::size_t Sub, typename Seq1, typename Seq2, std::size_t... Sizes>
struct SubStorage;

template<typename T, std::size_t Sub, size_t... I1, std::size_t... I2, std::size_t... Sizes>
struct SubStorage<T, Sub, std::index_sequence<I1...>, std::index_sequence<I2...>, Sizes...>
{
	// template<template <typename, std::size_t...> typename T, typename R>
	// constexpr static auto call(const T<R, Sizes...>& obj, const R& ref)
	// {
	// 	constexpr auto dimTuple = std::make_tuple(Sizes...);
	// 	auto ptr = &ref;
	// 	using Ret = T<const R&, (std::get<Sub + I1>(dimTuple), ...)>;
	// 	return Ret{{(ptr + I2)...}};
	// }

	template<typename S, typename R>
	constexpr static auto call(S& storage, R& ref)
	{
		constexpr auto dimTuple = std::make_tuple(Sizes...);
		using Ret = typename T::template Rebind<R&, (std::get<Sub + I1>(dimTuple), ...)>;
		return Ret{{(&ref + I2)...}};
	}
};

/// DefaultStorage utility base template.
/// Can be used for linear storage implementations.
template<std::size_t... Sizes>
struct LinearStorage
{
public:
	using Size = std::size_t;

	/// The number of dimensions this Storage has.
	static constexpr auto dim = sizeof...(Sizes);

	/// The total number of elements this storage holds.
	static constexpr auto totalSize = (Sizes * ...);

	/// A std::tuple with the dimension sizes for easier access.
	static constexpr auto dimTuple = std::make_tuple(Sizes...);

public:
	/// Returns the absolute position of the given coordinates inside the table.
	/// If the number of arguments is less than the dimension of this storage,
	/// returns the beginning of the associated substorage.
	/// Example: StackStorage<T, 2, 3, 4>::totalPosition(1, 0, 2)
	///   returns (1 * (3 * 4)) + (0 * 4) + (2 * 1) = 14.
	template<std::size_t Current = dim - 1, typename... Args>
	static constexpr auto linearPosition(Size head, Args... tail)
	{
		if constexpr(sizeof...(tail) == 0) return head * linearSize<Current>();
		else return linearSize<Current>() * head + linearPosition<Current - 1>(tail...);
	}

	/// Returns the total size of the array for the last \c LastDims dimensions.
	/// Example: StackStorage<T, 2, 3, 4>::totalSize<2>() returns (3 * 4) = 12.
	template<Size LastDims = dim>
	static constexpr auto linearSize()
	{
		if constexpr(LastDims == 0) return 1;
		else return std::get<dim - LastDims>(dimTuple) * linearSize<LastDims - 1>();
	}

	/// Returns the value or substorage for the given position and object.
	template<typename T, typename... Args>
	static constexpr decltype(auto) at(T& obj, Args... pos)
	{
		auto lp = linearPosition(pos...);
		decltype(auto) storage = obj.storage();

		if constexpr(sizeof...(pos) == dim) return storage[lp];
		else
		{
			using Seq1 = std::make_index_sequence<dim - sizeof...(pos)>;
			using Seq2 = std::make_index_sequence<linearSize<sizeof...(pos)>()>;
			using SStorage = SubStorage<T, sizeof...(pos), Seq1, Seq2, Sizes...>;
			return SStorage::call(storage, storage[lp]);
		}
	}
};



/// Storage implementation on the stack.
template<typename T, std::size_t... Sizes>
struct StackStorage
{
public:
	using Base = LinearStorage<Sizes...>;
	using Size = typename Base::Size;

	static constexpr auto totalSize = Base::totalSize;
	static constexpr auto dim = Base::dim;
	static constexpr auto dimTuple = Base::dimTuple;

	template<typename O, Size... OS>
	using Rebind = StackStorage<O, OS...>;

public:
	T storage_[totalSize];

public:
	constexpr auto* storage() { return storage_; }
	constexpr auto* storage() const { return storage_; }

	template<typename... Args> constexpr
	decltype(auto) at(Args... pos) const { return Base::at(*this, pos...); }

	template<typename... Args> constexpr
	decltype(auto) at(Args... pos) { return Base::at(*this, pos...); }

	// {
	// 	auto lp = Base::linearPosition(pos...);
	// 	if constexpr(sizeof...(pos) == dim)
	// 	{
	// 		return storage_[lp];
	// 	}
	// 	else
	// 	{
	// 		constexpr auto ls = Base::template linearSize<sizeof...(pos)>();
	// 		using Seq1 = std::make_index_sequence<dim - sizeof...(pos)>;
	// 		using Seq2 = std::make_index_sequence<ls>;
	// 		using SStorage = SubStorage<sizeof...(pos), Seq1, Seq2, Sizes...>;
	// 		return SStorage::call(*this, storage_[lp]);
	// 	}
	// }

	// template<typename... Args> constexpr
	// decltype(auto) at(Args... pos)
	// {
	// 	auto lp = Base::linearPosition(pos...);
	// 	if constexpr(sizeof...(pos) == dim)
	// 	{
	// 		return storage_[lp];
	// 	}
	// 	else
	// 	{
	// 		constexpr auto ls = Base::template linearSize<sizeof...(pos)>();
	// 		using Seq1 = std::make_index_sequence<dim - sizeof...(pos)>;
	// 		using Seq2 = std::make_index_sequence<ls>;
	// 		using SStorage = SubStorage<sizeof...(pos), Seq1, Seq2, Sizes...>;
	// 		return SStorage::call(*this, storage_[lp]);
	// 	}
	// }
};

template<typename T, std::size_t... Sizes>
struct StackStorage<T&, Sizes...>
{
public:
	using Base = LinearStorage<Sizes...>;
	using Size = typename Base::Size;
	static constexpr auto totalSize = Base::totalSize;
	static constexpr auto dim = Base::dim;
	static constexpr auto dimTuple = Base::dimTuple;

	template<typename O, Size... OS>
	using Rebind = StackStorage<O, OS...>;

public:
	T* storage_ [totalSize];

public:
	constexpr auto* storage() { return storage_; }
	constexpr auto* storage() const { return storage_; }

	template<typename... Args> constexpr
	decltype(auto) at(Args... pos) const { return *Base::at(*this, pos...); }

	template<typename... Args> constexpr
	decltype(auto) at(Args... pos) { return *Base::at(*this, pos...); }
};


static_assert(LinearStorage<2, 3, 4>::linearSize<1>() == 4);
static_assert(LinearStorage<2, 3, 4>::linearSize<2>() == 12);
static_assert(LinearStorage<2, 3, 4>::linearSize<3>() == 24);

static_assert(LinearStorage<2, 3, 4>::linearPosition(1, 0, 2) == 14);
static_assert(LinearStorage<2, 3, 4>::linearPosition(0, 0, 0) == 0);
static_assert(LinearStorage<2, 3, 4>::linearPosition(1, 2, 3) == 23);

//compile testing
void foo1()
{
	StackStorage<int, 2, 2> storage {{0, 1, 2, 4}};

	// static_assert(storage.at(0, 0) == 0);
	// static_assert(storage.at(0, 1) == 1);
	// static_assert(storage.at(1, 0) == 2);
	// static_assert(storage.at(1, 1) == 4);

	auto row0 = storage.at(0);
	assert(row0.at(0) == 0);
	assert(row0.at(1) == 1);

	auto row1 = storage.at(1);
	assert(row1.at(0) == 2);
	assert(row1.at(1) == 4);

	static_assert(std::is_same<decltype(storage.at(1, 0)), int&>::value);

	storage.at(1, 0) = 42;
	row1.at(1) = 83;

	std::cout << row1.at(0) << "\n";
	std::cout << row1.at(1) << "\n";
}

int main()
{
	foo1();

	// HeapStorage<int, 2, 3, 4> storage {{0, 1, 2}};
	// storage.at(0, 1) = 5;
	//
	// std::cout << storage.at(0, 0) << "\n";
	// std::cout << storage.at(0, 1) << "\n";
}

/// Storage implementation on the heap.
template<typename T, std::size_t... Sizes>
struct HeapStorage
{
public:
	using Base = LinearStorage<Sizes...>;
	static constexpr auto totalSize = Base::totalSize;

public:
	std::vector<T> storage_ = std::vector<T>(totalSize);

public:
	template<typename... Args> constexpr
	decltype(auto) at(Args... pos) const
	{
		return storage_[Base::linearPosition(pos...)];
	}

	template<typename... Args> constexpr
	decltype(auto) at(Args... pos)
	{
		return storage_[Base::linearPosition(pos...)];
	}
};

	// template<typename... Args>
	// static constexpr auto linearPositionEnd(Size head, Args... tail)
	// {
	// 	if constexpr(sizeof...(tail) == 0) return head;
	// 	else return linearSize<sizeof...(tail)>() * head + linearPosition(tail...);
	// }
