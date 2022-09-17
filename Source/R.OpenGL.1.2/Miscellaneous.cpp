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

#include "Graphics/Graphics.h"
#include "Module.h"
#include "State.h"

using namespace Renderer::Graphics;

namespace Renderer
{
    namespace External
    {
        extern "C" BOOL __cdecl CreateGraphicsCardList(s32 * count, char** apis, char** names, u32*, u32*)
        {
            *count = (s32)1;
            *apis = const_cast<char*>("OpenGL 1.2");
            *names = const_cast<char*>("OpenGL Compatible Graphics Card");

            return TRUE;
        }

        extern "C" BOOL __cdecl DrawPixels(void)
        {
            return FALSE;
        }

        extern "C" BOOL __cdecl SelectGraphicsCard(const u32)
        {
            return TRUE;
        }

        extern "C" BOOL __cdecl SetFog(const f32 r, const f32 g, const f32 b, const f32 start, const f32 end)
        {
            f32 color[] = { r, g, b, 0.0f };

            glFogi(GL_FOG_MODE, GL_LINEAR);
            glFogfv(GL_FOG_COLOR, color);
            glFogf(GL_FOG_START, start);
            glFogf(GL_FOG_END, end);

            return TRUE;
        }

        extern "C" BOOL __cdecl EnableClipping(const ClippingMode)
        {
            return TRUE;
        }

        extern "C" BOOL __cdecl EnableCulling(const CullMode mode)
        {
            State.GL.Mode.Cull = mode;

            return TRUE;
        }

        extern "C" BOOL __cdecl Clear(const u32, const u32 color)
        {
            GL::SetMode(GRAPHICS_MODE_IS_DEPTH_BUFFER_WRITES_ENABLED | GRAPHICS_MODE_IS_DEPTH_BUFFER_ENABLED);

            auto r = 0.003921569f * (color >> 16 & 0xff);
            auto g = 0.003921569f * (color >> 8 & 0xff);
            auto b = 0.003921569f * (color & 0xff);

            glClearColor(r, g, b, 1.0f);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            return TRUE;
        }

        extern "C" BOOL __cdecl Toggle(void)
        {
            SwapBuffers(State.Window.DeviceContext);

            return TRUE;
        }

        extern "C" BOOL __cdecl SetAlpha(const u32 color)
        {
            glAlphaFunc(GL_GREATER, color * 0.003921569f);

            return TRUE;
        }

        extern "C" BOOL __cdecl SetGloss(const f32)
        {
            return FALSE;
        }

        extern "C" BOOL __cdecl SetColorTable(const u8*, u16*)
        {
            return FALSE;
        }

        extern "C" BOOL __cdecl HasShaderSupport(void)
        {
            return FALSE;
        }
    }
}