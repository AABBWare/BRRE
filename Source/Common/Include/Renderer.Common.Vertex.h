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
#include "Mathematics.Vector.h"

namespace Renderer
{
    struct D3DTLVertex
    {
        struct Mathematics::Vector4 XYZW;
        union
        {
            u32 RGBA;
            struct
            {
                u8 R;
                u8 G;
                u8 B;
                u8 A;
            };
        } Diffuse;
        u32 Specular;
        struct Mathematics::Vector2 UV;
    };

    struct D3DLVertex
    {
        struct Mathematics::Vector3 XYZ;
        u32 Diffuse;
        struct Mathematics::Vector2 UV;
    };

    struct SVertex
    {
        struct Mathematics::Vector3 XYZ;
        struct Mathematics::Vector3 Normal;
        struct Mathematics::Vector2 UV;
    };

    struct SVertex2Basis
    {
        struct Mathematics::Vector3 XYZ;
        struct Mathematics::Vector3 Normal;
        struct Mathematics::Vector2 UV;
        struct Mathematics::Vector2 UV1;
    };

    struct SVertexBasis
    {
        struct Mathematics::Vector3 XYZ;
        struct Mathematics::Vector3 Normal;
        struct Mathematics::Vector2 UV;
        struct Mathematics::Vector3 BlendWeight;
        struct Mathematics::Vector3 BlendIndexes;
        struct Mathematics::Vector2 UV1;
        struct Mathematics::Vector3 PointSize;
    };

    struct TLVertex
    {
        struct Mathematics::Vector3 XYZ;
        struct
        {
            f32 Z;
            u32 UnkOptions;
            f32 X;
            f32 Y;
        } Coordinates;
        struct Mathematics::Vector2 UV;
        struct Graphics::RGBA RGBA;
        f32 Unk1[3];
    };

    struct BoneVertex
    {
        u32 Unk;
        struct Mathematics::Vector3 XYZ;
        struct Mathematics::Vector3 BlendWeight;
        struct Mathematics::Vector3 BlendIndexes;
        struct Mathematics::Vector3 Normal;
        struct Mathematics::Vector4 PointSize;
        struct Mathematics::Vector3 Diffuse;
        struct Mathematics::Vector4 Specular;
        struct Mathematics::Vector2 UV;
        struct Mathematics::Vector3 UV1;
        struct Mathematics::Vector3 UV2;
    };
}