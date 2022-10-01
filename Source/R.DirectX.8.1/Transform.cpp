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

namespace Renderer::Module
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

        if (!State.Settings.IsFixedPipelineActive)
        {
            matrix.m33 = zFar / (zFar - zNear);
            matrix.m43 = -zNear * zFar / (zFar - zNear);
        }

        struct Matrix4x4 result;
        Multiply(&result, transform, &matrix);

        if (State.Settings.IsFixedPipelineActive)
        {
            struct Matrix4x4 inverted;
            Invert(&inverted, &State.DX.Transform.Matrix1);

            struct Matrix4x4 mt;
            Multiply(&mt, &inverted, &result);

            State.DX.DirectXDevice->SetTransform(D3DTRANSFORMSTATETYPE::D3DTS_TEXTURE0, (D3DMATRIX*)&mt);

            return TRUE;
        }

        CopyMemory(&State.DX.Transform.W2L, &result, sizeof(struct Matrix4x4));

        State.DX.Light.IsChanged = TRUE;

        return TRUE;
    }

    extern "C" BOOL __cdecl SetTransform(const struct Transform* transform, const f32 zNear, const f32 zFar)
    {
        if (State.Settings.IsFixedPipelineActive)
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

                State.DX.Transform.Projection.m11 = zNear;
                State.DX.Transform.Projection.m12 = 0.0f;
                State.DX.Transform.Projection.m13 = 0.0f;
                State.DX.Transform.Projection.m14 = 0.0f;

                State.DX.Transform.Projection.m21 = 0.0f;
                State.DX.Transform.Projection.m22 = zFar;
                State.DX.Transform.Projection.m23 = 0.0f;
                State.DX.Transform.Projection.m24 = 0.0f;

                State.DX.Transform.Projection.m31 = 0.0f;
                State.DX.Transform.Projection.m32 = 0.0f;
                State.DX.Transform.Projection.m33 = clippingFar / (clippingFar - clippingNear);
                State.DX.Transform.Projection.m34 = 1.0f;

                State.DX.Transform.Projection.m41 = 0.0f;
                State.DX.Transform.Projection.m42 = 0.0f;
                State.DX.Transform.Projection.m43 = -clippingNear * clippingFar / (clippingFar - clippingNear);
                State.DX.Transform.Projection.m44 = 0.0f;

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

        State.DX.Light.Color.R = transform->Light.Color.R;
        State.DX.Light.Color.G = transform->Light.Color.G;
        State.DX.Light.Color.B = transform->Light.Color.B;

        State.DX.Light.IsChanged = TRUE;

        if (State.Settings.IsFixedPipelineActive)
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

                light->Direction.x = px * transform->ModelView.m11 + py * transform->ModelView.m21 + pz * transform->ModelView.m31;
                light->Direction.y = px * transform->ModelView.m12 + py * transform->ModelView.m22 + pz * transform->ModelView.m32;
                light->Direction.z = px * transform->ModelView.m13 + py * transform->ModelView.m23 + pz * transform->ModelView.m33;

                light->Diffuse.r = length * State.DX.Light.Colors.R[x];
                light->Diffuse.g = length * State.DX.Light.Colors.G[x];
                light->Diffuse.b = length * State.DX.Light.Colors.B[x];
            }

            DX::SetLight();
        }

        return TRUE;
    }
}