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

namespace detail
{

//makeRefVec
template<typename seq> struct makeRefVec;
template<size_t... idx> struct makeRefVec<index_sequence<idx...>>
{
    template<size_t rows, size_t cols, typename prec>
    refVec<sizeof...(idx), prec> constexpr operator()(vec<rows, vec<cols, prec>>& v, size_t i) const
    {
        return refVec<sizeof...(idx), prec>(v[idx][i]...);
    }
};

//initMat
template<typename seq> struct initMatData;
template<size_t... idx> struct initMatData<index_sequence<idx...>>
{
    template<size_t rows, size_t cols, typename prec, typename... Args>
    void constexpr operator()(vec<rows, vec<cols, prec>>& v, Args... args) const
    {
        std::tuple<Args...> tup(args...);
        using expander = int[];
        expander{0, ((v[idx / cols][idx % cols] = std::get<idx>(tup)), 0)... };
    }
};

//copyMatData
template<typename seq> struct copyMatData;
template<size_t... idx> struct copyMatData<index_sequence<idx...>>
{
    template<size_t rows, size_t cols, typename prec>
    std::unique_ptr<prec[]> constexpr operator()(const vec<rows, vec<cols, prec>>& v) const
    {
        return std::unique_ptr<prec[]>(new prec[rows * cols]{v[idx / cols][idx % rows]...});
    }
};

}
