// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Provides the Typemap template class that allows psuedo-dynamic object creation.

#pragma once

#ifndef NYTL_INCLUDE_TYPEMAP
#define NYTL_INCLUDE_TYPEMAP

#include <nytl/fwd/typemap.hpp> // nytl::Typemap default template parameter
#include <unordered_map> // std::unordered_map
#include <typeinfo> // std::type_info
#include <memory> // std::unique_ptr
#include <type_traits> // std::conditional_t
#include <stdexcept> // std::invalid_argument
#include <any> // std::any

namespace nytl {
namespace detail {
	template<typename B, typename T> class TypemapFactory;
}

/// \brief Can be thought of as map (like std::map) that assoicates types to identifiers.
/// \details Internally just uses type erasure to "store" its types.
/// Therefore there are just a small set of operations that can then be done with those
/// types, like e.g. creating objects of them from an identifier or std::type_info or receive the
/// type_info of the stored type for a given identifier.
/// \tparam I Identifier type for the types (usually std::string to associate a name with a type).
/// \tparam B Base type of all stored types. Can be std::any or some custom base class for
/// all stores types (in which case std::unique_ptr<Base> is used to return the objects).
/// For std::any to work, all type registered must be CopyConstructibe.
/// Defaulted to std::any.
/// \tparam CA Additional construction arguments all types need to create an object.
/// \requires Type 'I' must Hashable (i.e. there must be a std::hash<I> implementation).
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

	/// Removes the type entry for the type 'T'.
	/// \returns Whether the was an entry for type 'T' that could be removed.
	/// \note This is a template member function with explicit type parameter.
	/// It can only be called like `typemap.template remove<Type>()`.
	/// Alternatively, the [nytl::remove(Typemap<B, I, CArgs...>&)]() helper function can be used.
	template<typename T>
	bool remove()
	{
		auto it = find(typeid(T));
		if(it != types_.cend()) {
			types_.erase(it);
			return true;
		}

		return false;
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

	/// Returns the type_info for the type associated with the given id.
	/// \return typeid(void) if the given id could not be found.
	const std::type_info& typeInfo(const Identifier& id) const
	{
		auto it = types_.find(id);
		if(it == types_.cend()) return typeid(void);
		return it->second->typeInfo();
	}

	/// Tries to find and return the identifier to the given type_info.
	/// To check if a given identifier/type_info exists, use the exists functions.
	/// \throws std::invalid_argument if there is no entry with the given type_info
	const Identifier& id(const std::type_info& info) const
	{
		auto it = find(info);
		if(it != types_.cend())
			return it->first;

		throw std::invalid_argument("nytl::Typemap::id: no entry with the given type_info");
	}

	/// Tries to find and return the Identifier for type 'T'.
	/// \exception std::invalid_argument if there is no entry for the given type
	template<typename T>
	const Identifier& id() const { return id(typeid(T)); }

	/// Returns whether there exists an type entry for the given identifier.
	bool exists(const Identifier& id) const { return types_.find(id) != types_.cend(); }

	/// Returns whether there exists an type entry for the given type info.
	bool exists(const std::type_info& info) const { return find(info) != types_.cend(); }

	/// Returns whether there exists an type entry for type 'T'.
	template<typename T>
	bool exists() const { return exists(typeid(T)); }

	/// Retrieves the interal map of type entries.
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

/// \brief Convinience function for registering types in a Typemap.
/// Requires since template member functions are unconvinient to call.
/// \module utility
template<typename T, typename Identifier, typename Base, typename ID, typename... CArgs>
void add(Typemap<Identifier, Base, CArgs...>& typemap, const ID& id)
{
	typemap.template add<T>(id);
}

/// \brief Convinience function for registering types in a Typemap.
/// Requires since template member functions are unconvinient to call.
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
		std::unique_ptr<B> call(Args... args) { return {new T(std::forward<Args>(args)...)}; }
	};

	template<typename T>
	struct TypemapFactory<std::any, T> {
		template<typename... Args> static
		std::any call(Args... args) { return {T(std::forward<Args>(args)...)}; }
	};
}

}

#endif //header guard
