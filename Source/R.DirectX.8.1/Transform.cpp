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

using namespace Mathematics;
using namespace Renderer::Graphics;

namespace Renderer
{
    namespace External
    {
        extern "C" BOOL __cdecl SetTransformO2W(const struct Matrix4x4* transform)
        {
            CopyMemory(&State.DX.Transform.O2W, transform, sizeof(struct Matrix4x4));

            State.DX.Light.IsChanged = TRUE;

            return TRUE;
        }

        extern "C" BOOL __cdecl SetTransformW2L(const struct Matrix4x4* transform, const f32 zNear, const f32 zFar)
        {
            struct Matrix4x4 matrix =
            {
                0.5f, 0.0f,  0.0f, 0.0f,
                0.0f, -0.5f, 0.0f, 0.0f,
                0.5f, 0.5f,  1.0f, 1.0f,
                0.0f, 0.0f,  0.0f, 0.0f
            };

            if (State.Settings.UseFixedFunctionPipe == 0)
            {
                matrix._33 = zFar / (zFar - zNear);
                matrix._43 = -zNear * zFar / (zFar - zNear);
            }

            struct Matrix4x4 result;
            MatrixMultiply(&result, transform, &matrix);

            if (State.Settings.UseFixedFunctionPipe != 0)
            {
                struct Matrix4x4 inverted;
                MatrixInvert(&inverted, &State.DX.Transform.Matrix1);

                struct Matrix4x4 mt;
                MatrixMultiply(&mt, &inverted, &result);

                State.DX.DirectXDevice->SetTransform(D3DTRANSFORMSTATETYPE::D3DTS_TEXTURE0, (D3DMATRIX*)&mt);

                return TRUE;
            }

            CopyMemory(&State.DX.Transform.W2L, &result, sizeof(struct Matrix4x4));

            State.DX.Light.IsChanged = TRUE;

            return TRUE;
        }

        extern "C" BOOL __cdecl SetTransform(const struct Transform* transform, const f32 zNear, const f32 zFar)
        {
            if (State.Settings.UseFixedFunctionPipe != 0)
            {
                State.DX.DirectXDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&transform->ModelView);
                State.DX.DirectXDevice->SetTransform(D3DTRANSFORMSTATETYPE::D3DTS_VIEW, (D3DMATRIX*)&State.DX.Transform.Identity);
            }

            if (State.DX.Transform.Near != zNear || State.DX.Transform.Far != zFar || State.DX.IsViewportSet)
            {
                State.DX.Transform.Near = zNear;
                State.DX.Transform.Far = zFar;

                if (State.DX.IsViewportSet)
                {
                    State.DX.IsViewportSet = FALSE;

                    auto clippingNear = State.DX.Mode.Clipping.Near;
                    auto clippingFar = State.DX.Mode.Clipping.Far;

                    State.DX.Transform.Projection._11 = zNear;
                    State.DX.Transform.Projection._12 = 0.0f;
                    State.DX.Transform.Projection._13 = 0.0f;
                    State.DX.Transform.Projection._14 = 0.0f;

                    State.DX.Transform.Projection._21 = 0.0f;
                    State.DX.Transform.Projection._22 = zFar;
                    State.DX.Transform.Projection._23 = 0.0f;
                    State.DX.Transform.Projection._24 = 0.0f;

                    State.DX.Transform.Projection._31 = 0.0f;
                    State.DX.Transform.Projection._32 = 0.0f;
                    State.DX.Transform.Projection._33 = clippingFar / (clippingFar - clippingNear);
                    State.DX.Transform.Projection._34 = 1.0f;

                    State.DX.Transform.Projection._41 = 0.0f;
                    State.DX.Transform.Projection._42 = 0.0f;
                    State.DX.Transform.Projection._43 = -clippingNear * clippingFar / (clippingFar - clippingNear);
                    State.DX.Transform.Projection._44 = 0.0f;

                    State.DX.DirectXDevice->SetTransform(D3DTRANSFORMSTATETYPE::D3DTS_PROJECTION, (D3DMATRIX*)&State.DX.Transform.Projection);
                }
            }

            CopyMemory(&State.DX.Transform.Matrix1, &transform->ModelView, sizeof(struct Matrix4x4));

            for (auto x = 0; x < MAX_LIGHT_COUNT; x++)
            {
                State.DX.Light.XYZ[x].X = transform->Light.XYZ->X;
                State.DX.Light.XYZ[x].Y = transform->Light.XYZ->Y;
                State.DX.Light.XYZ[x].Z = transform->Light.XYZ->Z;
            }

            State.DX.Transform.UnknownFloats4[0] = transform->V1.X;
            State.DX.Transform.UnknownFloats4[1] = transform->V1.Y;
            State.DX.Transform.UnknownFloats4[2] = transform->V1.Z;

            State.DX.Light.IsChanged = TRUE;

            if (State.Settings.UseFixedFunctionPipe != 0)
            {
                for (auto x = 0; x < MAX_LIGHT_COUNT; x++)
                {
                    auto item = &State.DX.Light.XYZ[x];
                    auto light = &State.DX.Light.Lights[x];

                    auto length = sqrtf(item->X * item->X + item->Y * item->Y + item->Z * item->Z);
                    if (length == 0.0f)
                    {
                        length = 1.0f;
                    }

                    auto px = item->X / length;
                    auto py = item->Y / length;
                    auto pz = item->Z / length;

                    light->Direction.x = px * transform->ModelView._11 + py * transform->ModelView._21 + pz * transform->ModelView._31;
                    light->Direction.y = px * transform->ModelView._12 + py * transform->ModelView._22 + pz * transform->ModelView._32;
                    light->Direction.z = px * transform->ModelView._13 + py * transform->ModelView._23 + pz * transform->ModelView._33;

                    light->Diffuse.r = length * State.DX.Light.Colors.R[x];
                    light->Diffuse.g = length * State.DX.Light.Colors.G[x];
                    light->Diffuse.b = length * State.DX.Light.Colors.B[x];
                }

                DX::SetLight();
            }

            return TRUE;
        }
    }
}