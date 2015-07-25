#pragma once

namespace util
{

//nonCopyable
class nonCopyable
{
private:
	nonCopyable(const nonCopyable&) = delete;
	nonCopyable& operator =(const nonCopyable&) = delete;
protected:
	nonCopyable() noexcept = default;
	nonCopyable(nonCopyable&) noexcept = default;

	nonCopyable(nonCopyable&&) noexcept = default;
	nonCopyable& operator=(nonCopyable&&) noexcept = default;
};

//nonMoveable
class nonMoveable
{
private:
	nonMoveable(const nonMoveable&) = delete;
	nonMoveable& operator =(const nonMoveable&) = delete;
	nonMoveable(nonMoveable&&) = delete;
	nonMoveable& operator=(nonMoveable&&) = delete;

protected:
	nonMoveable() noexcept = default;
	nonMoveable(nonMoveable&) noexcept = default;
};

}

