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

#pragma once

template<class P> using mat2 = squareMat<2,P>;
template<class P> using mat3 = squareMat<3,P>;
template<class P> using mat4 = squareMat<4,P>;
template<class P> using mat5 = squareMat<5,P>;

template<class P> using mat23 = mat<2,3,P>;
template<class P> using mat24 = mat<2,4,P>;
template<class P> using mat25 = mat<2,5,P>;

template<class P> using mat32 = mat<3,2,P>;
template<class P> using mat34 = mat<3,4,P>;
template<class P> using mat35 = mat<3,5,P>;

template<class P> using mat42 = mat<4,2,P>;
template<class P> using mat43 = mat<4,3,P>;
template<class P> using mat45 = mat<4,5,P>;

template<class P> using mat52 = mat<5,2,P>;
template<class P> using mat53 = mat<5,3,P>;
template<class P> using mat54 = mat<5,4,P>;

typedef mat2<float> mat2f;
typedef mat2<unsigned int> mat2ui;
typedef mat2<int> mat2i;
typedef mat2<double> mat2d;
typedef mat2<char> mat2c;
typedef mat2<unsigned char> mat2uc;
typedef mat2<long> mat2l;
typedef mat2<unsigned long> mat2ul;
typedef mat2<short> mat2s;
typedef mat2<unsigned short> mat2us;

typedef mat3<float> mat3f;
typedef mat3<unsigned int> mat3ui;
typedef mat3<int> mat3i;
typedef mat3<double> mat3d;
typedef mat3<char> mat3c;
typedef mat3<unsigned char> mat3uc;
typedef mat3<long> mat3l;
typedef mat3<unsigned long> mat3ul;
typedef mat3<short> mat3s;
typedef mat3<unsigned short> mat3us;

typedef mat4<float> mat4f;
typedef mat4<unsigned int> mat4ui;
typedef mat4<int> mat4i;
typedef mat4<double> mat4d;
typedef mat4<char> mat4c;
typedef mat4<unsigned char> mat4uc;
typedef mat4<long> mat4l;
typedef mat4<unsigned long> mat4ul;
typedef mat4<short> mat4s;
typedef mat4<unsigned short> mat4us;

typedef mat5<float> mat5f;
typedef mat5<unsigned int> mat5ui;
typedef mat5<int> mat5i;
typedef mat5<double> mat5d;
typedef mat5<char> mat5c;
typedef mat5<unsigned char> mat5uc;
typedef mat5<long> mat5l;
typedef mat5<unsigned long> mat5ul;
typedef mat5<short> mat5s;
typedef mat5<unsigned short> mat5us;


//
typedef mat23<float> mat23f;
typedef mat23<unsigned int> mat23ui;
typedef mat23<int> mat23i;
typedef mat23<double> mat23d;
typedef mat23<char> mat23c;
typedef mat23<unsigned char> mat23uc;
typedef mat23<long> mat23l;
typedef mat23<unsigned long> mat23ul;
typedef mat23<short> mat23s;
typedef mat23<unsigned short> mat23us;

typedef mat24<float> mat24f;
typedef mat24<unsigned int> mat24ui;
typedef mat24<int> mat24i;
typedef mat24<double> mat24d;
typedef mat24<char> mat24c;
typedef mat24<unsigned char> mat24uc;
typedef mat24<long> mat24l;
typedef mat24<unsigned long> mat24ul;
typedef mat24<short> mat24s;
typedef mat24<unsigned short> mat24us;

typedef mat25<float> mat25f;
typedef mat25<unsigned int> mat25ui;
typedef mat25<int> mat25i;
typedef mat25<double> mat25d;
typedef mat25<char> mat25c;
typedef mat25<unsigned char> mat25uc;
typedef mat25<long> mat25l;
typedef mat25<unsigned long> mat25ul;
typedef mat25<short> mat25s;
typedef mat25<unsigned short> mat25us;

typedef mat32<float> mat32f;
typedef mat32<unsigned int> mat32ui;
typedef mat32<int> mat32i;
typedef mat32<double> mat32d;
typedef mat32<char> mat32c;
typedef mat32<unsigned char> mat32uc;
typedef mat32<long> mat32l;
typedef mat32<unsigned long> mat32ul;
typedef mat32<short> mat32s;
typedef mat32<unsigned short> mat32us;

typedef mat34<float> mat34f;
typedef mat34<unsigned int> mat34ui;
typedef mat34<int> mat34i;
typedef mat34<double> mat34d;
typedef mat34<char> mat34c;
typedef mat34<unsigned char> mat34uc;
typedef mat34<long> mat34l;
typedef mat34<unsigned long> mat34ul;
typedef mat34<short> mat34s;
typedef mat34<unsigned short> mat34us;

typedef mat35<float> mat35f;
typedef mat35<unsigned int> mat35ui;
typedef mat35<int> mat35i;
typedef mat35<double> mat35d;
typedef mat35<char> mat35c;
typedef mat35<unsigned char> mat35uc;
typedef mat35<long> mat35l;
typedef mat35<unsigned long> mat35ul;
typedef mat35<short> mat35s;
typedef mat35<unsigned short> mat35us;

typedef mat42<float> mat42f;
typedef mat42<unsigned int> mat42ui;
typedef mat42<int> mat42i;
typedef mat42<double> mat42d;
typedef mat42<char> mat42c;
typedef mat42<unsigned char> mat42uc;
typedef mat42<long> mat42l;
typedef mat42<unsigned long> mat42ul;
typedef mat42<short> mat42s;
typedef mat42<unsigned short> mat42us;

typedef mat43<float> mat43f;
typedef mat43<unsigned int> mat43ui;
typedef mat43<int> mat43i;
typedef mat43<double> mat43d;
typedef mat43<char> mat43c;
typedef mat43<unsigned char> mat43uc;
typedef mat43<long> mat43l;
typedef mat43<unsigned long> mat43ul;
typedef mat43<short> mat43s;
typedef mat43<unsigned short> mat43us;

typedef mat45<float> mat45f;
typedef mat45<unsigned int> mat45ui;
typedef mat45<int> mat45i;
typedef mat45<double> mat45d;
typedef mat45<char> mat45c;
typedef mat45<unsigned char> mat45uc;
typedef mat45<long> mat45l;
typedef mat45<unsigned long> mat45ul;
typedef mat45<short> mat45s;
typedef mat45<unsigned short> mat45us;

typedef mat52<float> mat52f;
typedef mat52<unsigned int> mat52ui;
typedef mat52<int> mat52i;
typedef mat52<double> mat52d;
typedef mat52<char> mat52c;
typedef mat52<unsigned char> mat52uc;
typedef mat52<long> mat52l;
typedef mat52<unsigned long> mat52ul;
typedef mat52<short> mat52s;
typedef mat52<unsigned short> mat52us;

typedef mat53<float> mat53f;
typedef mat53<unsigned int> mat53ui;
typedef mat53<int> mat53i;
typedef mat53<double> mat53d;
typedef mat53<char> mat53c;
typedef mat53<unsigned char> mat53uc;
typedef mat53<long> mat53l;
typedef mat53<unsigned long> mat53ul;
typedef mat53<short> mat53s;
typedef mat53<unsigned short> mat53us;

typedef mat54<float> mat54f;
typedef mat54<unsigned int> mat54ui;
typedef mat54<int> mat54i;
typedef mat54<double> mat54d;
typedef mat54<char> mat54c;
typedef mat54<unsigned char> mat54uc;
typedef mat54<long> mat54l;
typedef mat54<unsigned long> mat54ul;
typedef mat54<short> mat54s;
typedef mat54<unsigned short> mat54us;
