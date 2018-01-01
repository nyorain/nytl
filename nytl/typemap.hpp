// Copyright (c) 2017-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Provides the Typemap template class that allows pseudo-dynamic object creation.

#pragma once

#ifndef NYTL_INCLUDE_TYPEMAP
#define NYTL_INCLUDE_TYPEMAP

#include <nytl/fwd/typemap.hpp> // nytl::Typemap default template parameter

#include <unordered_map> // std::unordered_map
#include <typeinfo> // std::type_info
#include <memory> // std::unique_ptr
#include <type_traits> // std::conditional_t
#include <stdexcept> // std::invalid_argument

namespace nytl {
namespace detail {
	template<typename B, typename T> struct TypemapFactory;
}

/// \brief Can be thought of as map (like std::map) that associates types to identifiers.
/// \details Internally just uses type erasure to "store" its types.
/// Therefore there are just a small set of operations that can then be done with those
/// types, like creating an object from the associated identifier or type_info or receive the
/// type_info of the stored type for a given identifier.
/// \tparam I Identifier type for the types (usually std::string to associate a name with a type).
/// \tparam B Base type of all stored types. Can be std::any or some custom base class for
/// all stores types (in which case std::unique_ptr<Base> is used to return the objects).
/// For std::any to work, all type registered must be CopyConstructible.
/// Defaulted to std::any.
/// \tparam CA Additional construction arguments all types need to create an object.
/// \requires Type 'I' must be Hashable (i.e. there must be a std::hash<I> implementation).
/// \module utility
template<typename I, typename B, typename... CArgs>
class Typemap {
public:
	using Base = B;
	using Identifier = I;
	using Object = typename std::conditional_t<std::is_same<Base, std::any>::value,
		std::any, std::unique_ptr<Base>>;

	template <typename T>
	using Factory = detail::TypemapFactory<B, T>;

	// type erase base interface
	struct TypeBase {
		virtual ~TypeBase() = default;
		virtual const std::type_info& typeInfo() const = 0;
		virtual Object create(CArgs... args) const = 0;
	};

	// type erase implementation for 'T'
	template<typename T>
	struct TypeImpl : TypeBase {
		const std::type_info& typeInfo() const override { return typeid(T); };
		Object create(CArgs... args) const override
		{
			return Factory<T>::call(std::forward<CArgs>(args)...);
		}
	};

public:
	/// Adds an entry that associates the given identifier to type 'T'
	/// \note This is a template member function with explicit type parameter.
	/// It can only be called like `typemap.template add<Type>()`.
	/// Alternatively, the [nytl::add(Typemap<B, I, CArgs...>&)]() helper function can be used.
	/// Overrides the entry if the type is already present.
	template<typename T>
	void add(const Identifier& id)
	{
		types_[id].reset(new TypeImpl<T>());
	}

	/// Removes the type entry for the given identifier.
	/// \returns Whether the was an entry for the given identifier that could be removed.
	bool remove(const Identifier& id)
	{
		auto it = types_.find(id);
		if(it != types_.cend()) {
			types_.erase(it);
			return true;
		}

		return false;
	}

	/// Removes all entries for the type 'T'.
	/// \returns Whether there was an entry for type 'T' that could be removed.
	/// \note This is a template member function with an explicit type parameter.
	/// It can only be called like `typemap.template remove<Type>()`.
	/// Alternatively, the [nytl::remove(Typemap<B, I, CArgs...>&)]() helper function can be used.
	template<typename T>
	unsigned int remove()
	{
		return remove(typeid(T));
	}

	/// Removes all entries for the given type_info.
	/// \returns The number of entries removed, e.g. 0 if no entry was removed.
	unsigned int remove(const std::type_info& id)
	{
		auto before = types_.size();

		// std::remove_if not supported for types_
		for(auto it = types_.begin(); it != types_.end();) {
			if(it->second->typeInfo() == id) it = types_.erase(it);
			else ++it;
		}

		return before - types_.size();
	}


	/// Creates a object for the given type identifier and construction arguments.
	/// \returns The created object wrapped into a std::unique_ptr<Base> or std::any object
	/// if Base is std::any. If the identifier was not found, an empty unique_ptr/any is returned.
	Object create(const Identifier& id, CArgs... args) const
	{
		auto it = types_.find(id);
		if(it != types_.cend())
			return it->second->create(std::forward<CArgs>(args)...);
		return {};
	}

	/// Creates a object for the given type_info and construction arguments.
	/// \returns The created object wrapped into a std::unique_ptr<Base> or std::any object
	/// if Base is std::any. If the type_info was not found, an empty unique_ptr/any is returned.
	Object create(const std::type_info& id, CArgs... args) const
	{
		auto it = types_.find(id);
		if(it != types_.cend())
			return it->second->create(std::forward<CArgs>(args)...);
		return {};
	}

	/// Returns the type_info for the type associated with the given id or typeid(void)
	/// if the given identifier is not registered.
	const std::type_info& typeInfo(const Identifier& id) const
	{
		auto it = types_.find(id);
		if(it == types_.cend()) return typeid(void);
		return it->second->typeInfo();
	}

	/// Tries to find and return the identifier to the given type_info.
	/// If type 'T' is registered multiple times, will return any of the
	/// associated identifiers.
	/// Returns nullptr if the identifier could not be found.
	const Identifier* id(const std::type_info& info) const
	{
		auto it = find(info);
		if(it != types_.cend())
			return &it->first;

		return nullptr;
	}

	/// Tries to find and return the Identifier for type 'T'.
	/// If type 'T' is registered multiple times, will return any of the
	/// associated identifiers.
	/// Returns nullptr if the given type is not registered.
	template<typename T>
	const Identifier* id() const { return id(typeid(T)); }

	/// Returns whether there exists an type entry for the given identifier.
	bool exists(const Identifier& id) const { return types_.find(id) != types_.cend(); }

	/// Returns whether there exists an type entry for the given type info.
	bool exists(const std::type_info& info) const { return find(info) != types_.cend(); }

	/// Returns whether there exists an type entry for type 'T'.
	template<typename T>
	bool exists() const { return exists(typeid(T)); }

	/// Returns the internal map of type entries.
	/// \returns A std::unordered_map<Identifier, std::unique_ptr<const TypeBase>> holding the
	/// stored types with it identifiers.
	const auto& types() const { return types_; }

protected:
	auto find(const std::type_info& id) const
	{
		for(auto it = types_.begin(); it != types_.end(); ++it)
			if(it->second->typeInfo() == id)
				return it;

		return types_.end();
	}

	auto find(const std::type_info& id)
	{
		for(auto it = types_.begin(); it != types_.end(); ++it)
			if(it->second->typeInfo() == id)
				return it;

		return types_.end();
	}

	std::unordered_map<Identifier, std::unique_ptr<const TypeBase>> types_; // stored types
};

/// \brief Convenience function for registering types in a Typemap.
/// Requires since template member functions are inconvenient to call.
/// Overrides the entry if the id is already present in the Typemap.
/// \module utility
template<typename T, typename Identifier, typename Base, typename ID, typename... CArgs>
void add(Typemap<Identifier, Base, CArgs...>& typemap, const ID& id)
{
	typemap.template add<T>(id);
}

/// \brief Convenience function for registering types in a Typemap.
/// Requires since template member functions are inconvenient to call.
/// \module utility
template<typename T, typename Identifier, typename Base, typename... CArgs>
auto remove(Typemap<Identifier, Base, CArgs...>& typemap)
{
	return typemap.template remove<T>();
}

namespace detail {
	template<typename B, typename T>
	struct TypemapFactory {
		template<typename... Args> static
		std::unique_ptr<B> call(Args... args)
			{ return std::make_unique<T>(std::forward<Args>(args)...); }
	};

	template<typename T>
	struct TypemapFactory<std::any, T> {
		template<typename... Args> static
		std::any call(Args... args) { return std::any{T(std::forward<Args>(args)...)}; }
	};
}

} // namespace nytl

#endif // header guard
