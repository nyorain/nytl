/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Jan Kelling
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

//tests///////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t dim, typename prec>
NYTL_CPP14_CONSTEXPR bool intersects(const region<dim, prec>& rega, const region<dim, prec>& regb)
{
    for(auto& r1 : rega.getRects())
    {
        for(auto& r2 : regb.getRects())
        {
            if(intersects(r1, r2)) return true;
        }
    }

    return false;
}
template<std::size_t dim, typename prec>
NYTL_CPP14_CONSTEXPR bool intersects(const region<dim, prec>& rega, const rect<dim, prec>& rectb)
{
    for(auto& r1 : rega.getRects())
    {
        if(intersects(r1, rectb)) return true;
    }

    return false;
}
template<std::size_t dim, typename prec>
NYTL_CPP14_CONSTEXPR bool intersects(const region<dim, prec>& rega, const line<dim, prec>& lineb)
{
    for(auto& rega : rega.getRects())
    {
        if(intersects(rega, lineb)) return true;
    }

    return false;
}
template<std::size_t dim, typename prec>
NYTL_CPP14_CONSTEXPR bool intersects(const region<dim, prec>& rega, const triangle<dim, prec>& trib)
{
    for(auto& rega : rega.getRects())
    {
        if(intersects(rega, trib)) return true;
    }

    return false;
}
template<std::size_t dim, typename prec> NYTL_CPP14_CONSTEXPR 
bool intersects(const rect<dim, prec>& a, const region<dim, prec>& b){ return intersects(b, a); }

template<std::size_t dim, typename prec> NYTL_CPP14_CONSTEXPR 
bool intersects(const line<dim, prec>& a, const region<dim, prec>& b){ return intersects(b, a); }

template<std::size_t dim, typename prec> NYTL_CPP14_CONSTEXPR 
bool intersects(const triangle<dim, prec>& a, const region<dim, prec>& b){ return intersects(b, a); }

//contains
template<std::size_t dim, typename prec>
NYTL_CPP14_CONSTEXPR bool contains(const region<dim, prec>& rega, const region<dim, prec>& regb)
{
    for(auto& r1 : rega.getRects())
    {
        for(auto& r2 : regb.getRects())
        {
            if(!contains(r1, r2)) return false;
        }
    }

    return true;
}
template<std::size_t dim, typename prec>
NYTL_CPP14_CONSTEXPR bool contains(const region<dim, prec>& rega, const rect<dim, prec>& rectb)
{
    for(auto& rega : rega.getRects())
    {
        if(!contains(rega, rectb)) return false;
    }

    return true;
}
template<std::size_t dim, typename prec>
NYTL_CPP14_CONSTEXPR bool contains(const region<dim, prec>& rega, const line<dim, prec>& lineb)
{
    for(auto& rega : rega.getRects())
    {
        if(!contains(rega, lineb)) return false;
    }

    return true;
}
template<std::size_t dim, typename prec>
NYTL_CPP14_CONSTEXPR bool contains(const region<dim, prec>& rega, const triangle<dim, prec>& trib)
{
    for(auto& rega : rega.getRects())
    {
        if(!contains(rega, trib)) return false;
    }

    return true;
}
template<std::size_t dim, typename prec>
NYTL_CPP14_CONSTEXPR bool contains(const region<dim, prec>& rega, const Vec<dim, prec>& Vecb)
{
    for(auto& r1 : rega.getRects())
    {
        if(contains(r1, Vecb)) return true;
    }

    return false;
}

template<std::size_t dim, typename prec> NYTL_CPP14_CONSTEXPR 
bool contains(const rect<dim, prec>& a, const region<dim, prec>& b)
{
	for(auto& r1 : b.getRects())
	{
		if(!contains(a, r1)) return false;
	}

	return true;
}

template<std::size_t dim, typename prec> NYTL_CPP14_CONSTEXPR 
bool contains(const triangle<dim, prec>& a, const region<dim, prec>& b)
{
	for(auto& r1 : b.getRects())
	{
		if(!contains(a, r1)) return false;
	}

	return true;
}

//functions/////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t dim, typename prec>
void region<dim, prec>::add(const rect<dim, prec>& r)
{
    std::vector<rect<dim, prec>> rVec{r};
    for(auto& r1 : rects_)
    {
		std::size_t size = rVec.size();
        for(std::size_t i(0); i < size; ++i)
        {
            if(intersects(r1, rVec[i]))
            {
                auto result = difference(rVec[i], r1);
                if(!result.empty())
                {
                    rVec[i] = result[0];
                    rVec.insert(rVec.cend(), result.cbegin() + 1, result.cend());
                }
                else
                {
                    rVec.erase(rVec.cbegin() + i);
                    --i;
                }
            }
        }
    }

    if(!rVec.empty()) rects_.insert(rects_.cend(), rVec.cbegin(), rVec.cend());
}

template<std::size_t dim, typename prec>
void region<dim, prec>::add(const region<dim, prec>& r)
{
    for(auto& r1 : r.getRects())
        add(r1);
}

template<std::size_t dim, typename prec>
void region<dim, prec>::subtract(const rect<dim, prec>& r)
{
	std::vector<rect<dim,prec>> rVec{r};
	std::vector<rect<dim, prec>> addVec{};

    for(auto it = rects_.begin(); it != rects_.end(); ++it)
    {
		auto& r1 = *it;

		std::size_t size = rVec.size();
		for(std::size_t i(0); i < size; ++i)
		{
			if(intersects(r1, rVec[i]))
			{
				auto result = difference(rVec[i], r1);
				if(!result.empty())
				{
					r1 = result[0];
					addVec.insert(addVec.cend(), result.cbegin() + 1, result.cend());				
				}
				else
				{
					it = rects_.erase(it);
				}
			}
		}
    }
}

template<std::size_t dim, typename prec>
void region<dim, prec>::subtract(const region<dim, prec>& r)
{
    for(auto& r1 : r.getRects())
        subtract(r1);
}

//operators/////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t dim, typename prec>
region<dim, prec>& region<dim, prec>::operator|=(const rect<dim, prec>& r)
{
    add(r);
    return *this;
}

template<std::size_t dim, typename prec>
region<dim, prec>& region<dim, prec>::operator|=(const region<dim, prec>& r)
{
    add(r);
    return *this;
}

template<std::size_t dim, typename prec>
region<dim, prec>& region<dim, prec>::operator&=(const rect<dim, prec>& r)
{
    std::vector<rect<dim,prec>> newRects_;
    for(auto& rec : rects_)
    {
        if(intersects(rec, r)) newRects_.push_back(r & rec);
    }

    rects_ = std::move(newRects_);
    return *this;
}

template<std::size_t dim, typename prec>
region<dim, prec>& region<dim, prec>::operator&=(const region<dim, prec>& r)
{
    std::vector<rect<dim,prec>> newRects_;
    for(auto& rec : rects_)
    {
        for(auto& rec2 : r.rects_)
        {
            if(intersects(rec, rec2)) newRects_.push_back(rec & rec2);
        }
    }

    rects_ = std::move(newRects_);
    return *this;
}

template<std::size_t dim, typename prec>
region<dim, prec>& region<dim, prec>::operator^=(const rect<dim, prec>& r)
{
    auto reg = r & *this;
    add(r);
    subtract(reg);

    return *this;
}

template<std::size_t dim, typename prec>
region<dim, prec>& region<dim, prec>::operator^=(const region<dim, prec>& r)
{
    auto reg = r & *this;
    add(r);
    subtract(reg);

    return *this;
}

//external operators/////////////////////////////////////////////////////////////////////////////////////////////
//move needed because it is a expression, not an identifier
template<std::size_t dim, typename prec>
NYTL_CPP14_CONSTEXPR region<dim, prec> operator|(region<dim, prec> a, const region<dim, prec>& b)
{
    return std::move(a |= b);
}

template<std::size_t dim, typename prec>
NYTL_CPP14_CONSTEXPR region<dim, prec> operator|(region<dim, prec> a, const rect<dim, prec>& b)
{
    return std::move(a |= b);
}

template<std::size_t dim, typename prec>
NYTL_CPP14_CONSTEXPR region<dim, prec> operator&(region<dim, prec> a, const region<dim, prec>& b)
{
    return std::move(a &= b);
}

template<std::size_t dim, typename prec>
NYTL_CPP14_CONSTEXPR region<dim, prec> operator&(region<dim, prec> a, const rect<dim, prec>& b)
{
    return std::move(a &= b);
}


template<std::size_t dim, typename prec>
NYTL_CPP14_CONSTEXPR region<dim, prec> operator^(region<dim, prec> a, const region<dim, prec>& b)
{
    return std::move(a ^= b);
}

template<std::size_t dim, typename prec>
NYTL_CPP14_CONSTEXPR region<dim, prec> operator^(region<dim, prec> a, const rect<dim, prec>& b)
{
    return std::move(a ^= b);
}
