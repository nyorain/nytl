/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Jan Kelling
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

///\file
///\brief Provides utility classes for creating singletong types.

#pragma once

namespace nytl
{

//singleton
template<typename T>
class singleton
{
public:
    static T& instance()
    {
        static T instance;
        return instance;
    }

protected:
    ~singleton() = default;
};

//dynamicSingleton
template<typename T>
class dynamicSingleton
{
protected:
    static T* singletonInstance(T* newPtr = nullptr, bool change = 0)
    {
        static T* instancePtr = nullptr;
        if(change) instancePtr = newPtr;
        return instancePtr;
    }

public:
    static T* instance()
    {
        return singletonInstance();
    }

protected:
	dynamicSingleton(){ if(!instance()) singletonInstance(this, 1); }
    ~dynamicSingleton(){ if(instance() == this) singletonInstance(nullptr, 1); };
};

}
