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

extern "C" u32 __cdecl SetViewport(const f32 x, const f32 y, const f32 width, const f32 height, const f32 zNear, const f32 zFar)
{
    D3DVIEWPORT8 vp =
    {
        .X = (u32)x,
        .Y = (u32)y,
        .Width = (u32)(width - x),
        .Height = (u32)(height - y),
        .MinZ = 0.0f,
        .MaxZ = 1.0f
    };

    State.DX.DirectXDevice->SetViewport(&vp);

    State.DX.Mode.Clipping.Near = zNear;
    State.DX.Mode.Clipping.Far = zFar;

    State.DX.IsViewportSet = TRUE;

    return TRUE;
}

extern "C" u32 __cdecl BeginCubeMapRender(const u32 indx, const u32 side)
{
    if (State.Settings.CubeTextureCount <= indx) { return FALSE; }

    auto face = (D3DCUBEMAP_FACES)side;

    if (face == D3DCUBEMAP_FACES::D3DCUBEMAP_FACE_POSITIVE_X)
    {
        State.DX.DirectXDevice->GetRenderTarget(&State.DX.Surfaces.DrawSurface);
        State.DX.DirectXDevice->GetDepthStencilSurface(&State.DX.Surfaces.DepthSurface);
    }

    auto texture = State.DX.Textures.Cube.Textures[indx];

    IDirect3DSurface8* surface;
    texture->GetCubeMapSurface(face, 0, &surface);

    State.DX.DirectXDevice->SetRenderTarget(surface, State.DX.Surfaces.CubeTextureDepthSurface);

    surface->Release();

    State.DX.Textures.Cube.CurrentSide = face;

    return TRUE;
}

extern "C" u32 __cdecl RenderVertexBuffer(const void* buffer, const u32 vertexCount, const u16* indexes, const u32 indexCount, const u32 mode)
{
    DXSetMode(mode);

    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, State.DX.Mode.Cull);

    DXSetIndexes(indexes, indexCount);

    DXC(State.DX.DirectXDevice->SetStreamSource(0, (IDirect3DVertexBuffer8*)buffer, sizeof(struct SVertex)),
        "Unable to set stream source.");

    DXSetIndexSource(0, State.DX.Buffers.IndexBuffer);

    DXSetBlendShaders(mode);

    State.DX.DirectXDevice->DrawIndexedPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, vertexCount,
        State.DX.TriangleCount * 3, indexCount);

    State.DX.TriangleCount += indexCount;

    return TRUE;
}

extern "C" u32 __cdecl RenderVertexBufferBasis(const void* buffer, const u32 vertexCount, const u16* indexes, const u32 indexCount, const u32 mode)
{
    DXSetMode(mode);

    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, State.DX.Mode.Cull);

    DXSetIndexes(indexes, indexCount);

    DXC(State.DX.DirectXDevice->SetStreamSource(0, (IDirect3DVertexBuffer8*)buffer, sizeof(struct SVertexBasis)),
        "Unable to set stream source.");

    DXSetIndexSource(0, State.DX.Buffers.IndexBuffer);

    DXSetShadersMode(mode);

    State.DX.DirectXDevice->DrawIndexedPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, vertexCount,
        State.DX.TriangleCount * 3, indexCount);

    State.DX.TriangleCount += indexCount;

    return TRUE;
}

extern "C" u32 __cdecl RenderVertexBufferBone(const void* buffer, const u32 vertexCount, const u16* indexes, const u32 indexCount, const struct Vector4* constants, const u32 constantCount, const u32 mode)
{
    // todo: 24?
    if (24 <= constantCount) { return FALSE; }

    DXSetMode(mode);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, State.DX.Mode.Cull);

    DXSetIndexes(indexes, indexCount);

    DXC(State.DX.DirectXDevice->SetStreamSource(0, (IDirect3DVertexBuffer8*)buffer, sizeof(struct BoneVertex)),
        "Unable to set stream source.");

    DXSetIndexSource(0, State.DX.Buffers.IndexBuffer);

    DXSetSkeletonShaders(mode);

    for (auto x = 0; x < constantCount; x++)
    {
        // todo: 25?
        auto indx = 25 + x;// todo, is this correct? 25 + x * 3
        auto shaderConstants = &constants[x * 3];

        DXC(State.DX.DirectXDevice->SetVertexShaderConstant(indx, shaderConstants, 3),
            "Unable to set vertex shader constants.");
    }

    State.DX.DirectXDevice->DrawIndexedPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, vertexCount,
        State.DX.TriangleCount * 3, indexCount);

    State.DX.TriangleCount += indexCount;

    return TRUE;
}

extern "C" u32 __cdecl RenderVertexBufferPrelit(const void* buffer, const u32 vertexCount, const u16* indexes, const u32 indexCount, const u32 mode)
{
    DXSetMode(mode);

    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, State.DX.Mode.Cull);

    DXSetIndexes(indexes, indexCount);

    DXC(State.DX.DirectXDevice->SetStreamSource(0, (IDirect3DVertexBuffer8*)buffer, sizeof(struct SVertex)),
        "Unable to set stream source.");

    DXSetIndexSource(0, State.DX.Buffers.IndexBuffer);

    DXSetLightShaders(mode);

    State.DX.DirectXDevice->DrawIndexedPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, vertexCount,
        State.DX.TriangleCount * 3, indexCount);

    State.DX.TriangleCount += indexCount;

    return TRUE;
}

extern "C" u32 __cdecl EndCubeMapRender(void)
{
    if (State.DX.Textures.Cube.CurrentSide == D3DCUBEMAP_FACES::D3DCUBEMAP_FACE_NEGATIVE_Z)
    {
        State.DX.DirectXDevice->SetRenderTarget(State.DX.Surfaces.DrawSurface, State.DX.Surfaces.DepthSurface);

        State.DX.Surfaces.DrawSurface->Release();
        State.DX.Surfaces.DepthSurface->Release();
    }

    return TRUE;
}

extern "C" u32 __cdecl BeginRenderToTexture(u32 indx)
{
    if (State.Settings.RenderTextureCount <= indx) { return FALSE; }

    State.DX.DirectXDevice->GetRenderTarget(&State.DX.Surfaces.DrawSurface);
    State.DX.DirectXDevice->GetDepthStencilSurface(&State.DX.Surfaces.DepthSurface);

    IDirect3DSurface8* surface;
    State.DX.Textures.Render.Textures[indx]->GetSurfaceLevel(0, &surface);

    State.DX.DirectXDevice->SetRenderTarget(surface, State.DX.Surfaces.RenderTextureDepthSurface);

    surface->Release();

    return TRUE;
}

extern "C" u32 __cdecl EndRenderToTexture(void)
{
    State.DX.DirectXDevice->SetRenderTarget(State.DX.Surfaces.DrawSurface, State.DX.Surfaces.DepthSurface);

    State.DX.Surfaces.DrawSurface->Release();
    State.DX.Surfaces.DepthSurface->Release();

    return TRUE;
}

extern "C" u32 __cdecl SelectRenderTexture(const u32 indx)
{
    State.DX.Textures.Selected.Textures[0] = State.DX.Textures.Render.Textures[indx];

    return TRUE;
}