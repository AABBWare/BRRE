/*
Copyright (c) 2022 AABBWare

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "Win.h"

namespace DX::Shaders::Pixels
{
    extern const char NAME_BUMP[];
    extern const char NAME_BUMPSPEC[];
    extern const char NAME_DECAL[];
    extern const char NAME_DECALKIL[];
    extern const char NAME_PBMPCUBE[];
    extern const char NAME_PBMPSPLM[];
    extern const char NAME_PGLOSS[];
    extern const char NAME_PLMAP[];
    extern const char NAME_PLMBS[];

    extern const DWORD FUNC_BUMP[];
    extern const DWORD FUNC_BUMPSPEC[];
    extern const DWORD FUNC_DECAL[];
    extern const DWORD FUNC_DECALKIL[];
    extern const DWORD FUNC_PBMPCUBE[];
    extern const DWORD FUNC_PBMPSPLM[];
    extern const DWORD FUNC_PGLOSS[];
    extern const DWORD FUNC_PLMAP[];
    extern const DWORD FUNC_PLMBS[];
}