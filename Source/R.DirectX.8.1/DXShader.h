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

#include "Basic.h"
#include "Win.h"

struct DXVertexShader
{
    const DWORD* Declaration;
    const DWORD* Function;
    const char* Name;
    const DWORD FVF;
    DWORD Handle;
};

struct DXPixelShader
{
    const DWORD* Function;
    const char* Name;
    DWORD Handle;
};

enum class PixelShaderName : s32
{
    Decal = 0,
    Decalkil = 1,
    Bump = 2,
    Bumpspec = 3,
    Pbmpsplm = 4,
    Pbmpcube = 5,
    Pgloss = 6,
    Plmap = 7,
    Plmbs = 8
};

enum class VertexShaderName : s32
{
    Dirlight1 = 0,
    Texgen = 1,
    Cubic = 2,
    Prelit1 = 3,
    Prelit2 = 4,
    Pretex = 5,
    Vbump = 6,
    Vbmpspec = 7,
    Vbmpcube = 8,
    Vbumpgen = 9,
    Vbmpspgn = 10,
    Vbmpsppl = 11,
    Vbmpspom = 12,
    Vgloss = 13,
    Vlmap1 = 14,
    Vlmap2 = 15,
    Vskel = 16,
    Vskeltex = 17,
    Vomni1 = 18,
    Vomni2 = 19,
    Vomnin = 20,
    Vomnibmp = 21,
    Tlvertex = 22,
    Shdwbkfc1 = 23,
    Vspecmap = 24,
    Vskelspc = 25,
    Vskelsp2 = 26,
    Vskelsp3 = 27,
    Vskelbmp = 28,
    Vomni3 = 29,
    Dirlight2 = 30,
    Shdwbkfc2 = 31,
    Vlmbs = 32
};