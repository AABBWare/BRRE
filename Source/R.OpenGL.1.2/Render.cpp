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

using namespace Renderer::Graphics;

namespace Renderer
{
    namespace External
    {
        extern "C" BOOL __cdecl SetViewPort(const f32 x, const f32 y, const f32 width, const f32 height, const f32 zNear, const f32 zFar)
        {
            glViewport((s32)x, (s32)y, (s32)(width - x), (s32)(height - y));
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);

            State.GL.Mode.Clipping.Near = zNear;
            State.GL.Mode.Clipping.Far = zFar;

            return TRUE;
        }

        extern "C" BOOL __cdecl BeginCubeMapRender(const u32, const u32)
        {
            return FALSE;
        }

        extern "C" BOOL __cdecl RenderVertexBuffer(const void*, const u32, const u16*, const u32, const u32)
        {
            return FALSE;
        }

        extern "C" BOOL __cdecl RenderVertexBufferBasis(const void*, const u32, const u16*, const u32, const u32)
        {
            return FALSE;
        }

        extern "C" BOOL __cdecl RenderVertexBufferBone(const void*, const u32, const u16*, const u32, const struct Vector4*, const u32, const u32)
        {
            return FALSE;
        }

        extern "C" BOOL __cdecl RenderVertexBufferPrelit(const void*, const u32, const u16*, const u32, const u32)
        {
            return FALSE;
        }

        extern "C" BOOL __cdecl EndCubeMapRender(void)
        {
            return FALSE;
        }

        extern "C" BOOL __cdecl BeginRenderToTexture(const u32 indx)
        {
            glBindTexture(GL_TEXTURE_2D, State.GL.Textures.Render.Textures[indx].ID);
            GL::GLF.wglReleaseTexImageARB(State.GL.Textures.Render.Textures[indx].Buffer, WGL_FRONT_LEFT_ARB);
            wglMakeCurrent(State.GL.Textures.Render.Textures[indx].DeviceContext,
                State.GL.Textures.Render.Textures[indx].RenderContext);

            State.GL.Textures.Render.Index = indx;
            State.GL.Textures.Render.IsEnabled = TRUE;
            State.GL.Textures.Render.Direction = -1.0f;

            glDrawBuffer(GL_FRONT);
            glReadBuffer(GL_FRONT);

            return TRUE;
        }

        extern "C" BOOL __cdecl EndRenderToTexture(void)
        {
            glBindTexture(GL_TEXTURE_2D, State.GL.Textures.Render.Textures[State.GL.Textures.Render.Index].ID);
            wglMakeCurrent(State.Window.DeviceContext, State.Window.RenderContext);
            GL::GLF.wglBindTexImageARB(State.GL.Textures.Render.Textures[State.GL.Textures.Render.Index].Buffer, WGL_FRONT_LEFT_ARB);

            State.GL.Textures.Render.IsEnabled = FALSE;
            State.GL.Textures.Render.Direction = 1.0f;

            return TRUE;
        }

        extern "C" BOOL __cdecl SelectRenderTexture(const u32 indx)
        {
            State.GL.Textures.Selected.Textures[0] = State.GL.Textures.Render.Textures[indx].ID;

            return TRUE;
        }
    }
}