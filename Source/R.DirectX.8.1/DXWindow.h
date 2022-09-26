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

#include <d3dx8.h>

namespace Renderer
{
    namespace External
    {
        namespace DX
        {
            namespace Window
            {
                struct DXWindow
                {
                    u32 Width;
                    u32 Height;
                    u32 BitsPerPixel;

                    u32* Unknown4; // todo

                    HWND HWND;
                    IDirect3DSurface8* DepthSurface;
                    IDirect3DSwapChain8* SwapChain;
                };

                void Initialize(struct DXWindow*);
                void Initialize(struct DXWindow*, D3DPRESENT_PARAMETERS*);
                BOOL Initialize(const u32, const u32, const u32);
            }
        }
    }
}