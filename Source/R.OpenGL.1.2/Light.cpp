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

#include "Module.h"
#include "State.h"

extern "C" u32 __cdecl SetOmniLight(const f32*, const f32, const u32)
{
    return FALSE;
}

extern "C" u32 __cdecl EnableLighting(const u32 mode)
{
    State.GL.Light.IsEnabled = (mode != 0);

    return TRUE;
}

extern "C" u32 __cdecl SetAmbientLight(const f32 value)
{
    return TRUE;
}

extern "C" u32 __cdecl SetAmbientLightColor(const f32 r, const f32 g, const f32 b)
{
    return TRUE;
}

extern "C" u32 __cdecl SetLightColor(const f32 r, const f32 g, const f32 b, const s32 index)
{
    State.GL.Light.Colors.R[index] = r;
    State.GL.Light.Colors.G[index] = g;
    State.GL.Light.Colors.B[index] = b;

    return TRUE;
}

extern "C" u32 __cdecl SetLightConstants(const f32, const f32, const f32, const f32 alpha)
{
    State.GL.Light.Colors.Alpha = alpha;

    return TRUE;
}

extern "C" u32 __cdecl SetLightVector(void)
{
    return TRUE;
}