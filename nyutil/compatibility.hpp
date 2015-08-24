#pragma once

#include <vector>
#include <functional>

namespace nyutil
{

template< typename > class compHandler;

template<typename Ret, typename ... Args> class compHandler<Ret(Args...)>
{
protected:
	std::vector<std::function<Ret(Args...)>> versions_;

public:
	void addVersion(std::function<Ret(Args...)> version){ versions_.push_back(version); };

	Ret call(unsigned int version, Args ... args){ if(version < versions_.size()) return versions_[version](args ...); return Ret(); }
	Ret operator()(unsigned int version, Args ... args){ return call(version, args ...); }
};

}
