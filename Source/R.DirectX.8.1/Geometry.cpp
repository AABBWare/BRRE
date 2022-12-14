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

#include "Exit.h"
#include "Module.h"
#include "State.h"

// todo: define 65000 and 10000, and 1000

using namespace Renderer::Enums;

namespace Renderer::Module
{
    BOOL DotLineListD3DTL(const struct D3DTLVertex* vertexes, const u32 vertexCount, const u32 mode, const D3DPRIMITIVETYPE primitive, const s32)
    {
        DX::SetMode(mode);

        if (1000 < vertexCount) { return FALSE; }

        CopyMemory(&State.DX.VertexesDL, vertexes, vertexCount * sizeof(struct D3DTLVertex));

        for (auto x = 0; x < vertexCount; x++)
        {
            auto normal = State.DX.VertexesDL[x].XYZW.W;
            // todo: is this correct?
            if (((normal < 0.0f) << 8 | (normal == 0.0f) << 14) == 0)
            {
                auto clippingNear = State.DX.Mode.Clipping.Near;
                auto clippingFar = State.DX.Mode.Clipping.Far;

                auto aspect = clippingFar / (clippingFar - clippingNear);
                State.DX.VertexesDL[x].XYZW.Z = ((1.0f / normal) * aspect - clippingNear * aspect) * normal;
            }
            else
            {
                State.DX.VertexesDL[x].XYZW.Z = 1.0f;
            }

            State.DX.IndexesDL[x] = x;
        }

        DX::DXC(State.DX.DirectXDevice->SetStreamSource(0, NULL, sizeof(struct D3DTLVertex)), "Unable to set stream source.");

        DX::SetShaders(DX::Shaders::DXVertexShaderName::Tlvertex, DX::Shaders::DXPixelShaderName::Decal);

        State.DX.DirectXDevice->DrawIndexedPrimitiveUP(primitive, 0, vertexCount, vertexCount, &State.DX.IndexesDL,
            D3DFORMAT::D3DFMT_INDEX16, &State.DX.VertexesDL, sizeof(struct D3DTLVertex));

        return TRUE;
    }

    extern "C" BOOL __cdecl DotListD3DTL(const struct D3DTLVertex* vertexes, const u32 vertexCount, const u32 mode)
    {
        return DotLineListD3DTL(vertexes, vertexCount, mode, D3DPRIMITIVETYPE::D3DPT_POINTLIST, vertexCount);
    }

    extern "C" BOOL __cdecl PolyListBasis(const struct SVertexBasis* vertexes, const u32 vertexCount, const u16 * indexes, const u32 indexCount, const u32 mode)
    {
        if (!State.DX.IsSceneActive) { return FALSE; }

        DX::SetMode(mode);

        DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, State.DX.Mode.Cull);

        if (65000 < vertexCount)
        {
            Exit("Too many vertexes.");
            return FALSE;
        }

        if (65000 < indexCount)
        {
            Exit("Too many indexes.");
            return FALSE;
        }

        if (!State.Settings.IsFixedPipelineActive)
        {
            if (65000 < State.DX.BasisVertexCount2 + vertexCount)
            {
                State.DX.BasisVertexCount2 = 0;
            }

            auto options = State.DX.BasisVertexCount2 == 0
                ? D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK
                : D3DLOCK_NOOVERWRITE | D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK;

            struct SVertexBasis* buffer;
            DX::DXC(State.DX.Buffers.SVertexBasisBuffer->Lock(State.DX.BasisVertexCount2 * sizeof(struct SVertexBasis),
                vertexCount * sizeof(struct SVertexBasis), (BYTE**)&buffer, options), "Unable to lock vertex buffer.");

            // todo: is this correct?
            CopyMemory(buffer, vertexes, vertexCount * sizeof(struct SVertexBasis));

            DX::DXC(State.DX.Buffers.SVertexBasisBuffer->Unlock(), "Unable to unlock vertex buffer.");

            DX::SetIndexes(indexes, indexCount);

            DX::DXC(State.DX.DirectXDevice->SetStreamSource(0, State.DX.Buffers.SVertexBasisBuffer, sizeof(struct SVertexBasis)),
                "Unable to set stream source.");

            DX::SetIndexSource(State.DX.BasisVertexCount2, State.DX.Buffers.IndexBuffer);

            DX::SetShadersMode(mode);

            DX::DXC(State.DX.DirectXDevice->DrawIndexedPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0,
                vertexCount, State.DX.TriangleCount * 3, indexCount), "Unable to draw indexed primitive.");

            State.DX.BasisVertexCount2 += vertexCount;
            State.DX.TriangleCount += indexCount;
        }
        else
        {
            if (65000 < State.DX.BasisVertexCount + vertexCount)
            {
                State.DX.BasisVertexCount = 0;
            }

            DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_LIGHTING, State.DX.Light.IsActive);

            auto options = State.DX.BasisVertexCount == 0
                ? D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK
                : D3DLOCK_NOOVERWRITE | D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK;

            struct SVertex2Basis* buffer;
            DX::DXC(State.DX.Buffers.SVertexBuffer->Lock(State.DX.BasisVertexCount * sizeof(struct SVertex2Basis),
                vertexCount * sizeof(struct SVertex2Basis), (BYTE**)&buffer, options), "Unable to lock vertex buffer.");

            for (auto x = 0; x < vertexCount; x++)
            {
                // todo: is this correct?
                auto vertex = &buffer[x];
                auto input = &vertexes[x];

                vertex->XYZ.X = input->XYZ.X;
                vertex->XYZ.Y = input->XYZ.Y;
                vertex->XYZ.Z = input->XYZ.Z;
                vertex->Normal.X = input->Normal.X;
                vertex->Normal.Y = input->Normal.Y;
                vertex->Normal.Z = input->Normal.Z;
                vertex->UV.X = input->UV.X;
                vertex->UV.Y = input->UV.Y;
                vertex->UV1.X = input->UV1.X;
                vertex->UV1.Y = input->UV1.Y;
            }

            DX::DXC(State.DX.Buffers.SVertexBuffer->Unlock(), "Unable to unlock vertex buffer.");

            DX::SetIndexes(indexes, indexCount);

            DX::DXC(State.DX.DirectXDevice->SetStreamSource(0, State.DX.Buffers.SVertexBuffer,
                sizeof(struct SVertex2Basis)), "Unable to set stream source.");

            DX::SetIndexSource(State.DX.BasisVertexCount, State.DX.Buffers.IndexBuffer);

            DX::SetBlendShaders(mode);

            DX::DXC(State.DX.DirectXDevice->DrawIndexedPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0,
                vertexCount, State.DX.TriangleCount * 3, indexCount), "Unable to draw indexed primitive.");

            State.DX.BasisVertexCount += vertexCount;
            State.DX.TriangleCount += indexCount;
        }

        return TRUE;
    }

    extern "C" BOOL __cdecl PolyListBone(const void*, const u32, const u16*, const u32, const u32, const u32)
    {
        return FALSE;
    }

    extern "C" BOOL __cdecl PolyListD3DL(const struct D3DTLVertex* vertexes, const u32 vertexCount, const u16 * indexes, const u32 indexCount, const u32 mode)
    {
        if (!State.DX.IsSceneActive) { return FALSE; }

        DX::SetMode(mode);

        DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, State.DX.Mode.Cull);
        DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_LIGHTING, FALSE);

        if (65000 < vertexCount)
        {
            Exit("Too many vertexes.");
            return FALSE;
        }

        if (65000 < indexCount)
        {
            Exit("Too many indexes.");
            return FALSE;
        }

        if (65000 < State.DX.D3DLVertexCount + vertexCount)
        {
            State.DX.D3DLVertexCount = 0;
        }

        auto options = State.DX.D3DLVertexCount == 0
            ? D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK
            : D3DLOCK_NOOVERWRITE | D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK;

        struct D3DLVertex* buffer;
        DX::DXC(State.DX.Buffers.D3DLVertexBuffer->Lock(State.DX.D3DLVertexCount * sizeof(struct D3DLVertex),
            vertexCount * sizeof(struct D3DLVertex), (BYTE**)&buffer, options), "Unable to lock vertex buffer.");

        for (auto x = 0; x < vertexCount; x++)
        {
            // todo: is this correct?
            auto vertex = &buffer[x];
            auto input = &vertexes[x];

            vertex->XYZ.X = input->XYZW.X;
            vertex->XYZ.Y = input->XYZW.Y;
            vertex->XYZ.Z = input->XYZW.Z;
            vertex->Diffuse = input->Diffuse.RGBA;
            vertex->UV.X = input->UV.X;
            vertex->UV.Y = input->UV.Y;
        }

        DX::DXC(State.DX.Buffers.D3DLVertexBuffer->Unlock(), "Unable to unlock vertex buffer.");

        DX::SetIndexes(indexes, indexCount);

        DX::DXC(State.DX.DirectXDevice->SetStreamSource(0, State.DX.Buffers.D3DLVertexBuffer,
            sizeof(struct D3DLVertex)), "Unable to set stream source.");

        DX::SetIndexSource(State.DX.D3DLVertexCount, State.DX.Buffers.IndexBuffer);

        DX::SetTLShaders(mode);

        DX::DXC(State.DX.DirectXDevice->DrawIndexedPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0,
            vertexCount, State.DX.TriangleCount * 3, indexCount), "Unable to draw indexed primitive.");

        State.DX.D3DLVertexCount += vertexCount;
        State.DX.TriangleCount += indexCount;

        return TRUE;
    }

    extern "C" BOOL __cdecl PolyListD3DTL(const struct D3DTLVertex* vertexes, const u32 vertexCount, const u16 * indexes, const u32 indexCount, const u32 mode)
    {
        if (!State.DX.IsSceneActive) { return FALSE; }

        DX::SetMode(mode & 0xffffff37);

        DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, State.DX.Mode.Cull);

        if (10000 < vertexCount)
        {
            Exit("Too many vertexes.");
            return FALSE;
        }

        if (65000 < indexCount)
        {
            Exit("Too many indexes.");
            return FALSE;
        }

        if (10000 < State.DX.VertexCount + vertexCount)
        {
            State.DX.VertexCount = 0;
        }

        auto options = State.DX.VertexCount == 0
            ? D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK
            : D3DLOCK_NOOVERWRITE | D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK;

        struct D3DTLVertex* buffer;
        DX::DXC(State.DX.Buffers.D3DTLVertexBuffer->Lock(State.DX.VertexCount * sizeof(struct D3DTLVertex),
            vertexCount * sizeof(struct D3DTLVertex), (BYTE**)&buffer, options), "Unable to lock vertex buffer.");

        CopyMemory(buffer, vertexes, vertexCount * sizeof(struct D3DTLVertex));

        DX::DXC(State.DX.Buffers.D3DTLVertexBuffer->Unlock(), "Unable to unlock vertex buffer.");

        DX::SetIndexes(indexes, indexCount);

        DX::DXC(State.DX.DirectXDevice->SetStreamSource(0, State.DX.Buffers.D3DTLVertexBuffer,
            sizeof(struct D3DTLVertex)), "Unable to set stream source.");

        DX::SetIndexSource(State.DX.VertexCount, State.DX.Buffers.IndexBuffer);

        DX::SetShaders(DX::Shaders::DXVertexShaderName::Tlvertex, DX::Shaders::DXPixelShaderName::Decal);

        DX::DXC(State.DX.DirectXDevice->DrawIndexedPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0,
            vertexCount, State.DX.TriangleCount * 3, indexCount), "Unable to draw indexed primitive.");

        State.DX.VertexCount += vertexCount;
        State.DX.TriangleCount += indexCount;

        return TRUE;
    }

    extern "C" BOOL __cdecl PolyList(const struct SVertex* vertexes, const u32 vertexCount, const u16 * indexes, const u32 indexCount, const u32 mode)
    {
        if (!State.DX.IsSceneActive) { return FALSE; }

        DX::SetMode(mode);

        DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, State.DX.Mode.Cull);

        if (State.Settings.IsFixedPipelineActive)
        {
            DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_LIGHTING, State.DX.Light.IsActive);
        }

        if (65000 < vertexCount)
        {
            Exit("Too many vertexes.");
            return FALSE;
        }

        if (65000 < indexCount)
        {
            Exit("Too many indexes.");
            return FALSE;
        }

        if (65000 < State.DX.BasisVertexCount + vertexCount)
        {
            State.DX.BasisVertexCount = 0;
        }

        auto options = State.DX.BasisVertexCount == 0
            ? D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK
            : D3DLOCK_NOOVERWRITE | D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK;

        if (!State.Settings.IsFixedPipelineActive)
        {
            struct SVertex* buffer;
            DX::DXC(State.DX.Buffers.SVertexBuffer->Lock(State.DX.BasisVertexCount * sizeof(struct SVertex),
                vertexCount * sizeof(struct SVertex), (BYTE**)&buffer, options), "Unable to lock vertex buffer.");

            CopyMemory(buffer, vertexes, vertexCount * sizeof(struct SVertex));
        }
        else
        {
            struct SVertex2Basis* buffer;
            DX::DXC(State.DX.Buffers.SVertexBuffer->Lock(State.DX.BasisVertexCount * sizeof(struct SVertex2Basis),
                vertexCount * sizeof(struct SVertex2Basis), (BYTE**)&buffer, options), "Unable to lock vertex buffer.");

            for (auto x = 0; x < vertexCount; x++)
            {
                // todo: is this correct?
                auto vertex = &buffer[x];
                auto input = &vertexes[x];

                vertex->XYZ.X = input->XYZ.X;
                vertex->XYZ.Y = input->XYZ.Y;
                vertex->XYZ.Z = input->XYZ.Z;
                vertex->Normal.X = input->Normal.X;
                vertex->Normal.Y = input->Normal.Y;
                vertex->Normal.Z = input->Normal.Z;
                vertex->UV.X = input->UV.X;
                vertex->UV.Y = input->UV.Y;
                vertex->UV1.X = input->Normal.X;
                vertex->UV1.Y = input->Normal.Y;
            }
        }

        DX::DXC(State.DX.Buffers.SVertexBuffer->Unlock(), "Unable to unlock vertex buffer.");

        DX::SetIndexes(indexes, indexCount);

        DX::DXC(State.DX.DirectXDevice->SetStreamSource(0, State.DX.Buffers.SVertexBuffer,
            State.Settings.IsFixedPipelineActive ? sizeof(struct SVertex2Basis) : sizeof(struct SVertex)),
            "Unable to set stream source.");

        DX::SetIndexSource(State.DX.BasisVertexCount, State.DX.Buffers.IndexBuffer);

        DX::SetBlendShaders(mode);

        DX::DXC(State.DX.DirectXDevice->DrawIndexedPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0,
            vertexCount, State.DX.TriangleCount * 3, indexCount), "Unable to draw indexed primitive.");

        State.DX.BasisVertexCount += vertexCount;
        State.DX.TriangleCount += indexCount;

        return TRUE;
    }

    extern "C" BOOL __cdecl PolyListTL(const struct TLVertex* vertexes, const u32 vertexCount, const u16 * indexes, const u32 indexCount, const u32 mode)
    {
        if (!State.DX.IsSceneActive) { return FALSE; }

        if (State.DX.Textures.BlendOperation != BlendOperation::None) { return FALSE; }

        DX::SetMode(mode);

        DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, State.DX.Mode.Cull);
        DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_LIGHTING, FALSE);

        for (auto x = 0; x < vertexCount; x++)
        {
            // todo: is this correct?
            auto input = &vertexes[x];
            auto vertex = &State.DX.Vertexes[x];

            vertex->XYZW.X = input->Coordinates.X;
            vertex->XYZW.Y = input->Coordinates.Y;
            vertex->XYZW.Z = 1.0f - input->Coordinates.Z;

            vertex->Diffuse.RGBA = DX::Color(input->RGBA.B, input->RGBA.G, input->RGBA.R, input->RGBA.A);
            vertex->Specular = DX::Alpha(1.0f - input->Unk1[0]);

            if (mode & GRAPHICS_MODE_UNKNOWN_400)
            {
                vertex->XYZW.W = input->Unk1[1] * input->Coordinates.Z;

                vertex->UV.X = ((1.0f / input->Unk1[1]) * input->UV.X + 1.0f) * 0.5f;
                vertex->UV.Y = 0.5f - (1.0f / input->Unk1[1]) * input->UV.Y * 0.5f;
            }
            else
            {
                vertex->XYZW.W = input->Coordinates.Z;

                vertex->UV.X = input->UV.X;
                vertex->UV.Y = input->UV.Y;
            }
        }

        DX::SetShaders(DX::Shaders::DXVertexShaderName::Tlvertex, DX::Shaders::DXPixelShaderName::Decal);

        DX::DXC(State.DX.DirectXDevice->DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0,
            vertexCount, indexCount, indexes, D3DFORMAT::D3DFMT_INDEX16,
            &State.DX.Vertexes, sizeof(struct D3DTLVertex)), "Unable to draw indexed primitive.");

        return TRUE;
    }

    extern "C" BOOL __cdecl LineListD3DTL(const struct D3DTLVertex* vertexes, const u32 vertexCount, const u32 mode)
    {
        return DotLineListD3DTL(vertexes, vertexCount, mode, D3DPRIMITIVETYPE::D3DPT_LINELIST, vertexCount - 1);;
    }
}