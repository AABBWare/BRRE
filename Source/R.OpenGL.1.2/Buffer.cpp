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

#include "GL.h"
#include "Module.h"

namespace Renderer::Module
{
    extern "C" BOOL __cdecl ClearZBuffer(void)
    {
        GL::SetMode(GRAPHICS_MODE_IS_DEPTH_BUFFER_WRITES_ACTIVE | GRAPHICS_MODE_IS_DEPTH_BUFFER_ACTIVE);

        glClear(GL_DEPTH_BUFFER_BIT);

        return TRUE;
    }

    extern "C" BOOL __cdecl AllocateVertexBuffer(const void*, const u32, const u32, void**)
    {
        return FALSE;
    }

    extern "C" BOOL __cdecl FreeVertexBuffer(const void*)
    {
        return FALSE;
    }
}