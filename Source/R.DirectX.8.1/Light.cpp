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

#include "DX.h"
#include "Module.h"
#include "State.h"

using namespace Renderer::Graphics;

namespace Renderer
{
    namespace External
    {
        extern "C" BOOL __cdecl SetOmniLight(const f32* unk1, const f32 unk2, const u32 color)
        {
            // todo: is this correct?
            struct Vector4 constants[] =
            {
                {
                    (color & 0xff) * 0.003921569f,
                    (color >> 8 & 0xff) * 0.003921569f,
                    (color >> 16 & 0xff) * 0.003921569f,
                    1.0f,
                },
                {
                    unk1[0],
                    unk1[1],
                    unk1[2],
                    unk2
                }
            };

            DX::DXC(State.DX.DirectXDevice->SetVertexShaderConstant(25, &constants, ARRAYSIZE(constants)),
                "Unable to set vertex shader constants.");

            return TRUE;
        }

        extern "C" BOOL __cdecl EnableLighting(const BOOL mode)
        {
            State.DX.Light.IsEnabled = mode;

            return TRUE;
        }

        extern "C" BOOL __cdecl SetAmbientLight(const f32 color)
        {
            State.DX.Light.IsChanged = TRUE;

            State.DX.Light.Lights[0].Ambient.r = color;
            State.DX.Light.Lights[0].Ambient.g = color;
            State.DX.Light.Lights[0].Ambient.b = color;
            State.DX.Light.Lights[0].Ambient.a = color;

            return TRUE;
        }

        extern "C" BOOL __cdecl SetAmbientLightColor(const f32 r, const f32 g, const f32 b)
        {
            State.DX.Light.IsChanged = TRUE;

            State.DX.Light.Lights[0].Ambient.r = r;
            State.DX.Light.Lights[0].Ambient.g = g;
            State.DX.Light.Lights[0].Ambient.b = b;
            State.DX.Light.Lights[0].Ambient.a = 1.0f;

            return TRUE;
        }

        extern "C" BOOL __cdecl SetLightColor(const f32 r, const f32 g, const f32 b, const s32 indx)
        {
            State.DX.Light.Colors.R[indx] = r;
            State.DX.Light.Colors.G[indx] = g;
            State.DX.Light.Colors.B[indx] = b;

            return TRUE;
        }

        extern "C" BOOL __cdecl SetLightConstants(const f32 diffuse, const f32 specular, const f32 power, const f32 alpha)
        {
            D3DMATERIAL8 material =
            {
                .Diffuse =
                {
                    .r = diffuse,
                    .g = diffuse,
                    .b = diffuse,
                    .a = alpha
                },
                .Ambient =
                {
                    .r = 1.0f,
                    .g = 1.0f,
                    .b = 1.0f,
                    .a = alpha
                },
                .Specular =
                {
                    .r = specular,
                    .g = specular,
                    .b = specular,
                    .a = alpha
                },
                .Emissive =
                {
                    .r = 0.0f,
                    .g = 0.0f,
                    .b = 0.0f,
                    .a = 0.0f
                },
                .Power = power
            };

            if (State.Settings.UseFixedFunctionPipe != 0)
            {
                DX::DXC(State.DX.DirectXDevice->SetMaterial(&material), "Unable to set material.");
            }

            if (State.DX.Light.Alpha != alpha)
            {
                State.DX.Light.IsChanged = TRUE;
            }

            State.DX.Light.Alpha = alpha;

            return TRUE;
        }

        extern "C" BOOL __cdecl SetLightVector(void)
        {
            return TRUE;
        }
    }
}