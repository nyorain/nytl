#pragma once

#include <unordered_map>
#include <functional>

namespace nyutil
{

template<typename Version, typename> 
class versionHandler;

template<typename Version, typename Ret, typename ... Args> 
class versionHandler<Version, Ret(Args...)>
{
protected:
	std::unordered_map<Version, std::function<Ret(Args...)>> versions_;

public:
	void addVersion(Version version, std::function<Ret(Args...)> func){ versions_[version] = func; };
	bool hasVersion(Version version) const { return versions_.find(version) != versions_.cend(); }

	Ret call(Version version, Args ... args){ return versions_[version](args ...); }
	Ret operator()(Version version, Args ... args){ return call(version, args ...); }
};

}
