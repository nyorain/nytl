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

using expander = int[];

//makeRowRefVec
template<std::size_t size, typename seq = make_index_sequence<size>> struct makeRowRefVec;
template<std::size_t size, size_t... idx> struct makeRowRefVec<size, index_sequence<idx...>>
{
    template<size_t rows, size_t cols, typename prec>
    static refVec<sizeof...(idx), prec> call(Vec<rows, Vec<cols, prec>>& v, size_t i)
    {
        return refVec<sizeof...(idx), prec>(v[idx][i]...);
    }
};

//makeRowVec
template<std::size_t size, typename seq = make_index_sequence<size>> struct makeRowVec;
template<std::size_t size, size_t... idx> struct makeRowVec<size, index_sequence<idx...>>
{
    template<size_t rows, size_t cols, typename prec>
    static Vec<sizeof...(idx), prec> call(const Vec<rows, Vec<cols, prec>>& v, size_t i)
    {
        return Vec<sizeof...(idx), prec>(v[idx][i]...);
    }
};

//initMat
template<std::size_t size, typename seq = make_index_sequence<size>> struct initMatData;
template<std::size_t size, std::size_t... idx> struct initMatData<size, index_sequence<idx...>>
{
    template<size_t rows, size_t cols, typename prec, typename... Args>
    static void call(Vec<rows, Vec<cols, prec>>& v, std::tuple<Args...> args)
    {
        unused(expander{0, ((v[idx / cols][idx % cols] = std::get<idx>(args)), 0)... }[0]);
    }

    template<size_t rows, size_t cols, typename prec>
    static void call(Vec<rows, Vec<cols, prec>>& v, const prec& val)
    {
        unused(expander{0, ((v[idx / cols][idx % cols] = val), 0)... }[0]);
    }
};

//copyMatData
template<std::size_t size, typename seq = make_index_sequence<size>> struct copyMatData;
template<std::size_t size, size_t... idx> struct copyMatData<size, index_sequence<idx...>>
{
    template<size_t rows, size_t cols, typename prec>
    static std::unique_ptr<prec[]> call(const Vec<rows, Vec<cols, prec>>& v)
    {
        return std::unique_ptr<prec[]>(new prec[rows * cols]{v[idx / cols][idx % rows]...});
    }
};

//makeRowRefVec
template<std::size_t size, typename seq = make_index_sequence<size>> struct matTuple;
template<std::size_t size, size_t... idx> struct matTuple<size, index_sequence<idx...>>
{
    template<size_t rows, size_t cols, typename P>
    static type_tuple_t<P, sizeof...(idx)> call(const Vec<rows, Vec<cols, P>>& v)
    {
        return type_tuple_t<P, sizeof...(idx)>{v[idx / cols][idx % rows]...};
    }
};

}
