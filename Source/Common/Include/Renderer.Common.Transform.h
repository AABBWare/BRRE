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
#include "Graphics.Color.h"
#include "Graphics.h"
#include "Mathematics.Matrix.h"
#include "Mathematics.Vector.h"
#include "Renderer.Common.h"
#include "Renderer.Common.Vertex.h"

namespace Renderer
{
    struct Transform
    {
        struct Mathematics::Matrix4x4 UnknownMatrix;//todo
        struct
        {
            struct Graphics::RGBA Color;
            struct Mathematics::Vector4 XYZ[MAX_LIGHT_COUNT];
        } Light;
        struct Mathematics::Matrix4x4 ModelView;
    };
}