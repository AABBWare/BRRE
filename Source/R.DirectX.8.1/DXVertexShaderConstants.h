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

namespace Renderer
{
    namespace External
    {
        namespace DX
        {
            namespace Shader
            {
                namespace Vertex
                {
                    extern const DWORD DECLARATION_XYZ_NORMAL_UV[];
                    extern const DWORD DECLARATION_XYZ_DIFFUSE_UV[];
                    extern const DWORD DECLARATION_UNKNOWN[];
                    extern const DWORD DECLARATION_XYZRHW_DIFFUSE_SPECULAR_UV[];
                    extern const DWORD DECLARATION_SKELETON[];
                    extern const DWORD DECLARATION_BASIS[];

                    extern const char NAME_CUBIC[];
                    extern const char NAME_DIRLIGHT[];
                    extern const char NAME_PRELIT[];
                    extern const char NAME_PRETEX[];
                    extern const char NAME_SHDWBKFC[];
                    extern const char NAME_TEXGEN[];
                    extern const char NAME_TLVERTEX[];
                    extern const char NAME_VBMPCUBE[];
                    extern const char NAME_VBMPSPEC[];
                    extern const char NAME_VBMPSPGN[];
                    extern const char NAME_VBMPSPOM[];
                    extern const char NAME_VBMPSPPL[];
                    extern const char NAME_VBUMP[];
                    extern const char NAME_VBUMPGEN[];
                    extern const char NAME_VGLOSS[];
                    extern const char NAME_VLMAP[];
                    extern const char NAME_VLMAP2[];
                    extern const char NAME_VLMBS[];
                    extern const char NAME_VOMNI[];
                    extern const char NAME_VOMNIBMP[];
                    extern const char NAME_VOMNIN[];
                    extern const char NAME_VSKEL[];
                    extern const char NAME_VSKELBMP[];
                    extern const char NAME_VSKELSPC[];
                    extern const char NAME_VSKELSPC2[];
                    extern const char NAME_VSKELSPC3[];
                    extern const char NAME_VSKELTEX[];
                    extern const char NAME_VSPECMAP[];

                    extern const DWORD FUNC_CUBIC[];
                    extern const DWORD FUNC_DIRLIGHT[];
                    extern const DWORD FUNC_PRELIT[];
                    extern const DWORD FUNC_PRETEX[];
                    extern const DWORD FUNC_SHDWBKFC[];
                    extern const DWORD FUNC_TEXGEN[];
                    extern const DWORD FUNC_VBMPCUBE[];
                    extern const DWORD FUNC_VBMPSPEC[];
                    extern const DWORD FUNC_VBMPSPGN[];
                    extern const DWORD FUNC_VBMPSPOM[];
                    extern const DWORD FUNC_VBMPSPPL[];
                    extern const DWORD FUNC_VBUMP[];
                    extern const DWORD FUNC_VBUMPGEN[];
                    extern const DWORD FUNC_VGLOSS[];
                    extern const DWORD FUNC_VLMAP[];
                    extern const DWORD FUNC_VLMAP2[];
                    extern const DWORD FUNC_VLMBS[];
                    extern const DWORD FUNC_VOMNI[];
                    extern const DWORD FUNC_VOMNIBMP[];
                    extern const DWORD FUNC_VOMNIN[];
                    extern const DWORD FUNC_VSKEL[];
                    extern const DWORD FUNC_VSKELBMP[];
                    extern const DWORD FUNC_VSKELSPC[];
                    extern const DWORD FUNC_VSKELSPC2[];
                    extern const DWORD FUNC_VSKELSPC3[];
                    extern const DWORD FUNC_VSKELTEX[];
                    extern const DWORD FUNC_VSPECMAP[];
                }
            }
        }
    }
}