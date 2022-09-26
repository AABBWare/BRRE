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
#include "State.h"

#include <math.h>

using namespace Mathematics;
using namespace Renderer::Graphics;

namespace Renderer
{
    namespace External
    {
        extern "C" BOOL __cdecl SetTransformO2W(const struct Matrix4x4*)
        {
            return FALSE;
        }

        extern "C" BOOL __cdecl SetTransformW2L(const struct Matrix4x4*, const f32, const f32)
        {
            return FALSE;
        }

        extern "C" BOOL __cdecl SetTransform(const struct Transform* transform, const f32 zNear, const f32 zFar)
        {
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glLoadMatrixf((float*)&transform->ModelView);

            auto clippingNear = State.GL.Mode.Clipping.Near;
            auto clippingFar = State.GL.Mode.Clipping.Far;

            State.GL.Transform.Projection.m11 = zNear;
            State.GL.Transform.Projection.m12 = 0.0f;
            State.GL.Transform.Projection.m13 = 0.0f;
            State.GL.Transform.Projection.m14 = 0.0f;

            State.GL.Transform.Projection.m21 = 0.0f;
            State.GL.Transform.Projection.m22 = State.GL.Textures.Render.Direction * zFar;
            State.GL.Transform.Projection.m23 = 0.0f;
            State.GL.Transform.Projection.m24 = 0.0f;

            State.GL.Transform.Projection.m31 = 0.0f;
            State.GL.Transform.Projection.m32 = 0.0f;
            State.GL.Transform.Projection.m33 = (clippingNear + clippingFar) / (clippingFar - clippingNear);
            State.GL.Transform.Projection.m34 = 1.0f;

            State.GL.Transform.Projection.m41 = 0.0f;
            State.GL.Transform.Projection.m42 = 0.0f;
            State.GL.Transform.Projection.m43 = 2.0f * clippingNear * clippingFar / (clippingNear - clippingFar);
            State.GL.Transform.Projection.m44 = 0.0f;

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glLoadMatrixf((float*)&State.GL.Transform.Projection);

            CopyMemory(&State.GL.Transform.ModelView, &transform->ModelView, sizeof(struct Matrix4x4));

            for (auto x = 0; x < MAX_LIGHT_COUNT; x++)
            {
                auto light = &State.GL.Light.Lights[x];
                auto position = &transform->Light.XYZ[x];

                auto length = sqrtf(position->X * position->X + position->Y * position->Y + position->Z * position->Z);
                if (length == 0.0f)
                {
                    length = 1.0f;
                }

                light->Position.X = -1.0f / length * position->X;
                light->Position.Y = -1.0f / length * position->Y;
                light->Position.Z = -1.0f / length * position->Z;
                light->Position.W = 0.0f;

                light->Color.R = length * State.GL.Light.Colors.R[x];
                light->Color.G = length * State.GL.Light.Colors.G[x];
                light->Color.B = length * State.GL.Light.Colors.B[x];
                light->Color.A = 1.0f;
            }

            glLightfv(GL_LIGHT0, GL_POSITION, (float*)&State.GL.Light.Lights[0].Position);
            glLightfv(GL_LIGHT1, GL_POSITION, (float*)&State.GL.Light.Lights[1].Position);
            glLightfv(GL_LIGHT2, GL_POSITION, (float*)&State.GL.Light.Lights[2].Position);

            glLightfv(GL_LIGHT0, GL_DIFFUSE, (float*)&State.GL.Light.Lights[0].Color);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, (float*)&State.GL.Light.Lights[1].Color);
            glLightfv(GL_LIGHT2, GL_DIFFUSE, (float*)&State.GL.Light.Lights[2].Color);

            return TRUE;
        }
    }
}