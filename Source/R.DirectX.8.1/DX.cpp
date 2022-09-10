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
#include "DX.h"
#include "Exit.h"
#include "Module.h"
#include "State.h"

#define MAX_TEXTURE_SURFACE_SIZE 1024

u32 DXAlpha(const f32 a)
{
    return ((u32)roundf(255.0f * a)) << 24;
}

u32 DXColor(const f32 b, const f32 g, const f32 r, const f32 a)
{
    auto rc = ((u32)roundf(255.0f * b) && 0xff) << 16;
    auto gc = ((u32)roundf(255.0f * g) && 0xff) << 8;
    auto bc = (u32)roundf(255.0f * r) && 0xff;
    auto ac = (u32)roundf(255.0f * a) << 24;

    return ac | gc | gc | ac;
}

void DXDestroy(void)
{
    for (auto x = 0; x < MAX_VERTEX_SHADER_COUNT; x++)
    {
        auto vs = &State.DX.Shaders.VertexShaders[x];

        if (vs->Handle != 0)
        {
            State.DX.DirectXDevice->DeleteVertexShader(vs->Handle);
            vs->Handle = 0;
        }
    }

    for (auto x = 0; x < MAX_PIXEL_SHADER_COUNT; x++)
    {
        auto ps = &State.DX.Shaders.PixelShaders[x];

        if (ps->Handle != 0)
        {
            State.DX.DirectXDevice->DeletePixelShader(ps->Handle);
            ps->Handle = 0;
        }
    }

    if (State.DX.Surfaces.Source565Surface != NULL)
    {
        State.DX.Surfaces.Source565Surface->Release();
        State.DX.Surfaces.Source565Surface = NULL;
    }

    if (State.DX.Surfaces.AlphaSourceSurface != NULL)
    {
        State.DX.Surfaces.AlphaSourceSurface->Release();
        State.DX.Surfaces.AlphaSourceSurface = NULL;
    }

    for (auto x = 0; x < State.Settings.CubeTextureCount; x++)
    {
        if (State.DX.Textures.Cube.Textures[x] != NULL)
        {
            State.DX.Textures.Cube.Textures[x]->Release();
            State.DX.Textures.Cube.Textures[x] = NULL;
        }
    }

    for (auto x = 0; x < State.Settings.RenderTextureCount; x++)
    {
        if (State.DX.Textures.Render.Textures[x] != NULL)
        {
            State.DX.Textures.Render.Textures[x]->Release();
            State.DX.Textures.Render.Textures[x] = NULL;
        }
    }

    if (State.DX.Surfaces.RenderTextureDepthSurface != NULL)
    {
        State.DX.Surfaces.RenderTextureDepthSurface->Release();
        State.DX.Surfaces.RenderTextureDepthSurface = NULL;
    }

    if (State.DX.Surfaces.CubeTextureDepthSurface != NULL)
    {
        State.DX.Surfaces.CubeTextureDepthSurface->Release();
        State.DX.Surfaces.CubeTextureDepthSurface = NULL;
    }

    for (auto x = 1; x < MAX_WINDOW_COUNT; x++)
    {
        auto dxw = State.DX.Windows.Windows[x];

        if (dxw != NULL)
        {
            if (dxw->SwapChain != NULL)
            {
                dxw->SwapChain->Release();
                dxw->SwapChain = NULL;
            }

            if (dxw->DepthSurface != NULL)
            {
                dxw->DepthSurface->Release();
                dxw->DepthSurface = NULL;
            }
        }
    }

    ResetTextureCache();

    if (State.DX.Buffers.SVertexBuffer != NULL)
    {
        State.DX.Buffers.SVertexBuffer->Release();
        State.DX.Buffers.SVertexBuffer = NULL;
    }

    if (State.DX.Buffers.D3DLVertexBuffer != NULL)
    {
        State.DX.Buffers.D3DLVertexBuffer->Release();
        State.DX.Buffers.D3DLVertexBuffer = NULL;
    }

    if (State.DX.Buffers.D3DTLVertexBuffer != NULL)
    {
        State.DX.Buffers.D3DTLVertexBuffer->Release();
        State.DX.Buffers.D3DTLVertexBuffer = NULL;
    }

    if (State.DX.Buffers.SVertexBasisBuffer != NULL)
    {
        State.DX.Buffers.SVertexBasisBuffer->Release();
        State.DX.Buffers.SVertexBasisBuffer = NULL;
    }

    if (State.DX.Buffers.IndexBuffer != NULL)
    {
        State.DX.Buffers.IndexBuffer->Release();
        State.DX.Buffers.IndexBuffer = NULL;
    }

    ZeroMemory(State.DX.Shaders.Constants.Actual, ARRAYSIZE(State.DX.Shaders.Constants.Actual) * sizeof(Vector4));
}

u32 DXInitializeState(void)
{
    for (auto x = 0; x < ARRAYSIZE(State.DX.State); x++)
    {
        State.DX.State[x] = -1;
    }

    if (State.DX.DirectXDevice->BeginScene()) { return FALSE; }

    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_FILLMODE, D3DFILLMODE::D3DFILL_SOLID);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_SHADEMODE, D3DSHADEMODE::D3DSHADE_GOURAUD);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_LASTPIXEL, FALSE);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_ONE);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL::D3DCULL_NONE);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_DITHERENABLE, TRUE);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHABLENDENABLE, FALSE);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGENABLE, FALSE);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGCOLOR, 0);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_SPECULARENABLE, TRUE);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZVISIBLE, FALSE);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZENABLE, D3DZBUFFERTYPE::D3DZB_FALSE);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZWRITEENABLE, FALSE);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_ALWAYS);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMPFUNC::D3DCMP_GREATER);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_CLIPPING, TRUE);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_LIGHTING, FALSE);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILZFAIL, D3DSTENCILOP::D3DSTENCILOP_KEEP);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILFAIL, D3DSTENCILOP::D3DSTENCILOP_KEEP);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILREF, 1);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILMASK, -1);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILWRITEMASK, -1);

    for (auto x = 0; x < ARRAYSIZE(State.DX.Textures.UnknownArray); x++)
    {
        State.DX.Textures.UnknownArray[x] = NULL;
    }

    for (auto x = 0; x < MAX_SELECTED_TEXTURE_COUNT; x++)
    {
        State.DX.Textures.Selected.Textures[x] = NULL;
    }

    State.DX.Mode.Mode = 0;

    return State.DX.DirectXDevice->EndScene() == 0;
}

void DXCreate(void)
{
    for (auto x = 0; x < ARRAYSIZE(State.DX.State); x++)
    {
        State.DX.State[x] = 0;
    }

    for (auto x = 0; x < MAX_SELECTED_TEXTURE_COUNT; x++)
    {
        for (auto xx = 0; xx < MAX_TEXTURE_STAGE_COUNT; xx++)
        {
            State.DX.Textures.Selected.Stages[x][xx] = 0;
        }
    }

    if (State.Settings.UseFixedFunctionPipe == 0)
    {
        for (auto x = 0; x < MAX_VERTEX_SHADER_COUNT; x++)
        {
            auto shader = &State.DX.Shaders.VertexShaders[x];

            if (shader->Function != NULL)
            {
                DXC(State.DX.DirectXDevice->CreateVertexShader(shader->Declaration,
                    shader->Function, &shader->Handle, 0), "Unable to create vertex shader.");
            }
        }

        for (auto x = 0; x < MAX_PIXEL_SHADER_COUNT; x++)
        {
            auto shader = &State.DX.Shaders.PixelShaders[x];

            DXC(State.DX.DirectXDevice->CreatePixelShader(shader->Function, &shader->Handle),
                "Unable to create pixel shader.");
        }
    }

    D3DVIEWPORT8 viewport =
    {
        .X = 0,
        .Y = 0,
        .Width = (DWORD)State.DX.Windows.Current->Width,
        .Height = (DWORD)State.DX.Windows.Current->Height,
        .MinZ = 0.0f,
        .MaxZ = 1.0f
    };

    State.DX.DirectXDevice->SetViewport(&viewport);

    if (State.DX.Windows.Current->BitsPerPixel == BITS_PER_PIXEL_16)
    {
        State.DX.Surfaces.AlphaSourceSurfaceFormat = D3DFORMAT::D3DFMT_A4R4G4B4;
        State.DX.Surfaces.DrawSurfaceFormat = TextureFormat::Color16Bit4444;
    }
    else
    {
        State.DX.Surfaces.AlphaSourceSurfaceFormat = D3DFORMAT::D3DFMT_A8R8G8B8;
        State.DX.Surfaces.DrawSurfaceFormat = TextureFormat::Color32Bit;
    }

    DXC(State.DX.DirectXDevice->GetDepthStencilSurface(&State.DX.Windows.Windows[0]->DepthSurface),
        "Unable to get primary depth buffer.");

    if (State.DX.Windows.Windows[0]->DepthSurface != NULL)
    {
        State.DX.Windows.Windows[0]->DepthSurface->Release();
    }

    for (auto x = 1; x < MAX_WINDOW_COUNT; x++)
    {
        auto dxw = State.DX.Windows.Windows[x];

        if (dxw != NULL)
        {
            DXWindowInitialize(dxw->Width, dxw->Height, x);
        }
    }

    DXC(State.DX.DirectXDevice->CreateImageSurface(MAX_TEXTURE_SURFACE_SIZE, MAX_TEXTURE_SURFACE_SIZE,
        State.DX.Surfaces.AlphaSourceSurfaceFormat, &State.DX.Surfaces.AlphaSourceSurface),
        "Unable to create alpha source buffer.");

    DXC(State.DX.DirectXDevice->CreateImageSurface(MAX_TEXTURE_SURFACE_SIZE, MAX_TEXTURE_SURFACE_SIZE,
        D3DFORMAT::D3DFMT_R5G6B5, &State.DX.Surfaces.Source565Surface),
        "Unable to create 565 source buffer.");

    // todo: magic numbers below + types
    auto count = State.Settings.UseFixedFunctionPipe == 0 ? (65000 * 32) : (65000 * 40); // todo sizeof proper type

    DXC(State.DX.DirectXDevice->CreateVertexBuffer(count, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        State.Settings.UseFixedFunctionPipe == 0 ? D3DFVF_TEX0 : D3DFVF_TEX2 | D3DFVF_NORMAL | D3DFVF_XYZ,
        D3DPOOL::D3DPOOL_DEFAULT, &State.DX.Buffers.SVertexBuffer),
        "Unable to create vertex buffer.");

    DXC(State.DX.DirectXDevice->CreateVertexBuffer(65000 * sizeof(struct D3DLVertex),
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DFVF_TEX0, D3DPOOL::D3DPOOL_DEFAULT, &State.DX.Buffers.D3DLVertexBuffer),
        "Unable to create untransformed prelit vertex buffer.");

    DXC(State.DX.DirectXDevice->CreateVertexBuffer(10000 * sizeof(struct D3DTLVertex),
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DFVF_TEX1 | D3DFVF_SPECULAR | D3DFVF_DIFFUSE | D3DFVF_XYZRHW, D3DPOOL::D3DPOOL_DEFAULT,
        &State.DX.Buffers.D3DTLVertexBuffer),
        "Unable to create transformed prelit vertex buffer.");

    DXC(State.DX.DirectXDevice->CreateVertexBuffer(65000 * sizeof(struct SVertexBasis),
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DFVF_TEX0, D3DPOOL::D3DPOOL_DEFAULT, &State.DX.Buffers.SVertexBasisBuffer),
        "Unable to create vertex basis buffer.");

    DXC(State.DX.DirectXDevice->CreateIndexBuffer(65000 * 3 * sizeof(u16),
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DFORMAT::D3DFMT_INDEX16, D3DPOOL::D3DPOOL_SYSTEMMEM, &State.DX.Buffers.IndexBuffer),
        "Unable to create index buffer.");

    for (auto x = 0; x < State.Settings.CubeTextureCount; x++)
    {
        auto result = State.DX.DirectXDevice->CreateCubeTexture(State.Settings.CubeTextureSize, 1,
            D3DUSAGE_RENDERTARGET, State.DX.CurrentDevice.Capabilities.DevicePresentParameters.BackBufferFormat,
            D3DPOOL::D3DPOOL_DEFAULT, &State.DX.Textures.Cube.Textures[x]);

        if (result != 0)
        {
            State.Settings.CubeTextureCount = 0;
            break;
        }
    }

    for (auto x = 0; x < State.Settings.RenderTextureCount; x++)
    {
        auto result = State.DX.DirectXDevice->CreateTexture(State.Settings.RenderTextureSize, State.Settings.RenderTextureSize,
            1, D3DUSAGE_RENDERTARGET, State.DX.CurrentDevice.Capabilities.DevicePresentParameters.BackBufferFormat,
            D3DPOOL::D3DPOOL_DEFAULT, &State.DX.Textures.Render.Textures[x]);

        if (result != 0)
        {
            State.Settings.RenderTextureCount = 0;
            break;
        }
    }

    if (State.Settings.RenderTextureCount != 0)
    {
        DXC(State.DX.DirectXDevice->CreateDepthStencilSurface(State.Settings.RenderTextureSize, State.Settings.RenderTextureSize,
            State.DX.CurrentDevice.Capabilities.DevicePresentParameters.AutoDepthStencilFormat, D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_NONE,
            &State.DX.Surfaces.RenderTextureDepthSurface), "Unable to create render texture depth buffer.");
    }

    if (State.Settings.CubeTextureCount != 0)
    {
        DXC(State.DX.DirectXDevice->CreateDepthStencilSurface(State.Settings.CubeTextureSize, State.Settings.CubeTextureSize,
            State.DX.CurrentDevice.Capabilities.DevicePresentParameters.AutoDepthStencilFormat, D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_NONE,
            &State.DX.Surfaces.CubeTextureDepthSurface), "Unable to create cube texture depth buffer.");
    }

    if (DXInitializeState() == 0)
    {
        Exit("Unable to set DirectX into a known state.");
    }
}

u32 DXResetDevice(const s32 width, const s32 height, const s32 bpp, const s32 unk4, struct DXWindow* dxw)
{
    if (dxw == NULL) { return FALSE; }

    DXDestroy();

    dxw->Width = width;
    dxw->Height = height;
    dxw->BitsPerPixel = bpp;
    dxw->Unknown4 = unk4;

    // todo memcopy unk4

    DXWindowInitialize(dxw, &State.DX.CurrentDevice.Capabilities.DevicePresentParameters);

    DXC(State.DX.DirectXDevice->Reset(&State.DX.CurrentDevice.Capabilities.DevicePresentParameters),
        "Unable to reset device.");

    auto cdxw = State.DX.Windows.Current;
    State.DX.Windows.Current = dxw;

    DXCreate();

    State.DX.Windows.Current = cdxw;

    if (!State.Window.IsWindowMode)
    {
        ShowCursor(TRUE);
    }

    Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0);
    Toggle();

    return TRUE;
}

void DXSetRenderState(const D3DRENDERSTATETYPE state, const s32 value)
{
    if (State.DX.State[state] != value)
    {
        State.DX.State[state] = value;
        State.DX.DirectXDevice->SetRenderState(state, value);
    }
}

void DXSetMode(const u32 mode)
{
    // todo
    //DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_SRCBLEND,
    //    mode & GRAPHICS_MODE_IS_ALPHA_BLEND_ENABLED ? D3DBLEND::D3DBLEND_SRCALPHA : D3DBLEND::D3DBLEND_ONE);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_SRCBLEND, State.DX.Mode.Blend.SourceBlend);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_DESTBLEND, State.DX.Mode.Blend.DestinationBlend);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_BLENDOP, State.DX.Mode.Blend.DestinationBlendOperation);

    if ((State.DX.Mode.Mode ^ mode) & GRAPHICS_MODE_IS_TEXTURE_BLENDING_ENABLED)
    {
        if (mode & GRAPHICS_MODE_IS_TEXTURE_BLENDING_ENABLED)
        {
            State.DX.Textures.UnknownArray[0] = NULL;
        }
        else
        {
            DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_DISABLE);
        }
    }

    s32 indexes[] = { 0, -1, -1, -1 };

    switch (State.DX.Textures.BlendOperation)
    {
        case TextureBlendOperation::Disable:
        case TextureBlendOperation::BlendTextureAlpha:
        {
            indexes[0] = 1;
            indexes[1] = 0;
            break;
        }
        case TextureBlendOperation::SelectArgument1:
        case TextureBlendOperation::BlendFactorAlpha:
        case TextureBlendOperation::BlendTextureAlphaPreMultiplied:
        case TextureBlendOperation::BlendCurrentAlpha:
        {
            indexes[0] = 1;
            indexes[1] = 4;
            break;
        }
        case TextureBlendOperation::SelectArgument2:
        {
            indexes[0] = 2;
            break;
        }
        case TextureBlendOperation::Modulate:
        {
            indexes[0] = 1;
            indexes[3] = 2;
            break;
        }
        case TextureBlendOperation::Add:
        {
            indexes[0] = 0;
            indexes[1] = 1;
            indexes[2] = 2;
            break;
        }
        case TextureBlendOperation::AddSigned2X:
        {
            indexes[0] = 0;
            indexes[1] = 3;
            break;
        }
        case TextureBlendOperation::BlendDiffuseAlpha:
        {
            indexes[0] = 1;
            indexes[1] = 4;
            indexes[2] = 3;
            break;
        }
        case TextureBlendOperation::PreModulate:
        {
            indexes[0] = 1;
            indexes[1] = 4;
            indexes[2] = 0;
            indexes[3] = 3;
            break;
        }
    }

    IDirect3DBaseTexture8* tex[] = { NULL, NULL, NULL, NULL };

    for (auto x = 0; x < 4; x++)
    {
        if (indexes[x] >= 0)
        {
            tex[x] = State.DX.Textures.Selected.Textures[indexes[x]];
        }
    }

    if (tex[0] != State.DX.Textures.UnknownArray[0]
        || tex[1] != State.DX.Textures.UnknownArray[1]
        || tex[2] != State.DX.Textures.UnknownArray[2]
        || tex[3] != State.DX.Textures.UnknownArray[3]
        || State.DX.Textures.BlendOperation != State.DX.Textures.ClampOperation
        || ((State.DX.Mode.Mode ^ mode) & GRAPHICS_MODE_UNKNOWN_400))
    {
        for (auto x = 0; x < 4; x++)
        {
            State.DX.Textures.UnknownArray[x] = tex[x];
            State.DX.DirectXDevice->SetTexture(x, State.DX.Textures.UnknownArray[x]);

            if (State.DX.Textures.UnknownArray[x] != NULL)
            {
                DXSetTextureStageValue(x, D3DTEXTURESTAGESTATETYPE::D3DTSS_MAGFILTER, D3DTEXTUREFILTERTYPE::D3DTEXF_LINEAR);
                DXSetTextureStageValue(x, D3DTEXTURESTAGESTATETYPE::D3DTSS_MINFILTER, D3DTEXTUREFILTERTYPE::D3DTEXF_LINEAR);
                DXSetTextureStageValue(x, D3DTEXTURESTAGESTATETYPE::D3DTSS_MIPFILTER, D3DTEXTUREFILTERTYPE::D3DTEXF_LINEAR);

                auto indx = indexes[x];

                DXSetTextureStageValue(x, D3DTEXTURESTAGESTATETYPE::D3DTSS_ADDRESSU,
                    State.DX.Textures.Selected.ClampU[indx] == TextureClamp::Wrap
                    ? D3DTEXTUREADDRESS::D3DTADDRESS_WRAP : D3DTEXTUREADDRESS::D3DTADDRESS_CLAMP);
                DXSetTextureStageValue(x, D3DTEXTURESTAGESTATETYPE::D3DTSS_ADDRESSV,
                    State.DX.Textures.Selected.ClampV[indx] == TextureClamp::Wrap
                    ? D3DTEXTUREADDRESS::D3DTADDRESS_WRAP : D3DTEXTUREADDRESS::D3DTADDRESS_CLAMP);

                if (State.Settings.UseFixedFunctionPipe == 0)
                {
                    DXSetTextureStageValue(x, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXTURETRANSFORMFLAGS,
                        (mode & GRAPHICS_MODE_UNKNOWN_400)
                        ? D3DTEXTURETRANSFORMFLAGS::D3DTTFF_PROJECTED : D3DTEXTURETRANSFORMFLAGS::D3DTTFF_DISABLE);
                }
            }
        }

        if (State.DX.Textures.BlendOperation == TextureBlendOperation::None)
        {
            DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
            DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_MODULATE);
            DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLORARG1, D3DTA_TEXTURE);
            DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLORARG2, D3DTA_DIFFUSE);
            DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_MODULATE);
            DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
            DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

            DXSetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_DISABLE);
            DXSetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_DISABLE);

            DXSetTextureStageValue(2, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_DISABLE);
            DXSetTextureStageValue(2, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_DISABLE);

            DXSetTextureStageValue(3, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_DISABLE);
            DXSetTextureStageValue(3, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_DISABLE);
        }
        else if (State.DX.Textures.BlendOperation == TextureBlendOperation::AddSigned2X)
        {
            DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
            DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_MODULATE);
            DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLORARG1, D3DTA_TEXTURE);
            DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLORARG2, D3DTA_DIFFUSE);
            DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_MODULATE);
            DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
            DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

            DXSetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
            DXSetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_MODULATE);
            DXSetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLORARG1, D3DTA_TEXTURE);
            DXSetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLORARG2, D3DTA_CURRENT);
            DXSetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_MODULATE);
            DXSetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
            DXSetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAARG2, D3DTA_CURRENT);

            DXSetTextureStageValue(2, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_DISABLE);
            DXSetTextureStageValue(2, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_DISABLE);

            DXSetTextureStageValue(3, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_DISABLE);
            DXSetTextureStageValue(3, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_DISABLE);
        }

        if (State.Settings.UseFixedFunctionPipe != 0)
        {
            if (mode & GRAPHICS_MODE_UNKNOWN_400)
            {
                DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXTURETRANSFORMFLAGS,
                    D3DTEXTURETRANSFORMFLAGS::D3DTTFF_PROJECTED | D3DTEXTURETRANSFORMFLAGS::D3DTTFF_COUNT3);
                DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
            }
            else
            {
                if (State.DX.Textures.BlendOperation == TextureBlendOperation::SelectArgument2)
                {
                    DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXTURETRANSFORMFLAGS, D3DTEXTURETRANSFORMFLAGS::D3DTTFF_COUNT3);
                    DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR);
                }
                else
                {
                    DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXTURETRANSFORMFLAGS, D3DTEXTURETRANSFORMFLAGS::D3DTTFF_DISABLE);
                    DXSetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
                }
            }
        }
    }

    State.DX.Textures.ClampOperation = State.DX.Textures.BlendOperation;

    if ((State.DX.Mode.Mode ^ mode) & GRAPHICS_MODE_IS_ALPHA_BLEND_ENABLED)
    {
        if (mode & GRAPHICS_MODE_IS_ALPHA_BLEND_ENABLED)
        {
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHABLENDENABLE, TRUE);
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
        }
        else
        {
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHABLENDENABLE, FALSE);
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE);
        }
    }

    if ((State.DX.Mode.Mode ^ mode) & GRAPHICS_MODE_IS_FOG_ENABLED)
    {
        if (mode & GRAPHICS_MODE_IS_FOG_ENABLED)
        {
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGENABLE, TRUE);

            if (State.Settings.UseFixedFunctionPipe != 0)
            {
                DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGVERTEXMODE, D3DFOGMODE::D3DFOG_LINEAR);
                DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGTABLEMODE, D3DFOGMODE::D3DFOG_NONE);
            }
        }
        else
        {
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGENABLE, FALSE);

            if (State.Settings.UseFixedFunctionPipe != 0)
            {
                DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGVERTEXMODE, D3DFOGMODE::D3DFOG_NONE);
                DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGTABLEMODE, D3DFOGMODE::D3DFOG_NONE);
            }
        }
    }

    if (State.DX.IsStencilSet)
    {
        State.DX.Mode.Mode = mode;
        return;
    }

    if (!((State.DX.Mode.Mode ^ mode) & (GRAPHICS_MODE_IS_DEPTH_BUFFER_ENABLED | GRAPHICS_MODE_IS_DEPTH_BUFFER_WRITES_ENABLED)))
    {
        State.DX.Mode.Mode = mode;
        return;
    }

    if (mode & (GRAPHICS_MODE_IS_DEPTH_BUFFER_ENABLED | GRAPHICS_MODE_IS_DEPTH_BUFFER_WRITES_ENABLED))
    {
        DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZENABLE, State.DX.ZBufferType);

        if ((mode & GRAPHICS_MODE_IS_DEPTH_BUFFER_ENABLED) && (mode & GRAPHICS_MODE_IS_DEPTH_BUFFER_WRITES_ENABLED))
        {
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZWRITEENABLE, TRUE);
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_LESSEQUAL);
        }
        else if (mode & GRAPHICS_MODE_IS_DEPTH_BUFFER_ENABLED)
        {
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZWRITEENABLE, FALSE);
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_LESSEQUAL);
        }
        else
        {
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZWRITEENABLE, TRUE);
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_ALWAYS);
        }
    }
    else
    {
        DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZENABLE, D3DZBUFFERTYPE::D3DZB_FALSE);
        DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZWRITEENABLE, FALSE);
        DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_ALWAYS);
    }

    State.DX.Mode.Mode = mode;
}

void DXSetTextureStageValue(const u32 stage, const D3DTEXTURESTAGESTATETYPE type, const u32 value)
{
    if (State.DX.Textures.Selected.Stages[stage][type] != value)
    {
        State.DX.DirectXDevice->SetTextureStageState(stage, type, value);
        State.DX.Textures.Selected.Stages[stage][type] = value;
    }
}

void DXSetShaders(const VertexShaderName vsn, const PixelShaderName psn)
{
    auto vs = &State.DX.Shaders.VertexShaders[(s32)vsn];

    DXC(State.DX.DirectXDevice->SetVertexShader(vs->Handle == 0 ? vs->FVF : vs->Handle),
        "Unable to set vertex shader.");

    if (State.Settings.UseFixedFunctionPipe == 0)
    {
        DXC(State.DX.DirectXDevice->SetPixelShader(State.DX.Shaders.PixelShaders[(s32)psn].Handle),
            "Unable to set pixel shader.");
    }
}

void DXSetIndexSource(const u32 index, IDirect3DIndexBuffer8* buffer)
{
    DXC(State.DX.DirectXDevice->SetIndices(buffer, index), "Unable to set index source.");
}

void DXSetIndexes(const u16* indexes, const u32 count)
{
    if (65000 < count)
    {
        Exit("Too many indexes.");
    }

    if (65000 < State.DX.TriangleCount + count)
    {
        State.DX.TriangleCount = 0;
    }

    auto flags = State.DX.TriangleCount == 0
        ? D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK
        : D3DLOCK_NOOVERWRITE | D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK;

    u16* buffer;
    DXC(State.DX.Buffers.IndexBuffer->Lock(State.DX.TriangleCount * 3 * sizeof(u16),
        count * 3 * sizeof(u16), (BYTE**)&buffer, flags), "Unable to lock index buffer.");

    CopyMemory(buffer, indexes, count * 3 * sizeof(u16));

    DXC(State.DX.Buffers.IndexBuffer->Unlock(), "Unable to unlock index buffer.");
}

void DXSetLightShaders(const u32 mode)
{
    switch (State.DX.Textures.BlendOperation)
    {
        case TextureBlendOperation::Disable:
        {
            DXSetShaders(VertexShaderName::Prelit1, PixelShaderName::Bump);
            break;
        }
        case TextureBlendOperation::SelectArgument1:
        {
            DXSetShaders(VertexShaderName::Prelit1, PixelShaderName::Bumpspec);
            break;
        }
        case TextureBlendOperation::AddSigned:
        {
            DXSetShaders(VertexShaderName::Shdwbkfc1, PixelShaderName::Decalkil);
            break;
        }
        case TextureBlendOperation::Subtract:
        {
            DXSetShaders(VertexShaderName::Vomni1, PixelShaderName::Decal);
            break;
        }
        default:
        {
            if (mode & GRAPHICS_MODE_UNKNOWN_400)
            {
                DXSetShaders(VertexShaderName::Pretex, PixelShaderName::Decalkil);
            }
            else
            {
                DXSetShaders(VertexShaderName::Prelit1, PixelShaderName::Decal);
            }

            break;
        }
    }

    DXSetVertexShaderValues(0);
}

void DXSetSkeletonShaders(const u32 mode)
{
    switch (State.DX.Textures.BlendOperation)
    {
        case TextureBlendOperation::Disable:
        {
            DXSetShaders(VertexShaderName::Vskelbmp, PixelShaderName::Bump);
            break;
        }
        case TextureBlendOperation::SelectArgument1:
        {
            DXSetShaders(VertexShaderName::Vskelspc, PixelShaderName::Bumpspec);
            break;
        }
        case TextureBlendOperation::BlendTextureAlphaPreMultiplied:
        {
            DXSetShaders(VertexShaderName::Vskelsp2, PixelShaderName::Bumpspec);
            break;
        }
        case TextureBlendOperation::BlendCurrentAlpha:
        {
            DXSetShaders(VertexShaderName::Vskelsp3, PixelShaderName::Bumpspec);
            break;
        }
        default:
        {
            if (mode & GRAPHICS_MODE_UNKNOWN_400)
            {
                DXSetShaders(VertexShaderName::Vskeltex, PixelShaderName::Decalkil);
            }
            else
            {
                DXSetShaders(VertexShaderName::Vskel, PixelShaderName::Decal);
            }

            break;
        }
    }

    DXSetVertexShaderValues(0);
}

// todo: rename
void DXSetBlendShaders(const u32 mode)
{
    switch (State.DX.Textures.BlendOperation)
    {
        case TextureBlendOperation::None:
        {
            if (mode & GRAPHICS_MODE_UNKNOWN_400)
            {
                 DXSetShaders(VertexShaderName::Texgen, PixelShaderName::Decalkil);
            }
            else
            {
                DXSetShaders(VertexShaderName::Dirlight1, PixelShaderName::Decal);
            }

            break;
        }
        case TextureBlendOperation::SelectArgument2:
        {
            DXSetShaders(VertexShaderName::Cubic, PixelShaderName::Decal);
            break;
        }
        case TextureBlendOperation::Disable:
        case TextureBlendOperation::SelectArgument1:
        case TextureBlendOperation::Modulate:
        case TextureBlendOperation::BlendDiffuseAlpha:
        case TextureBlendOperation::BlendTextureAlpha:
        case TextureBlendOperation::BlendFactorAlpha:
        case TextureBlendOperation::BlendTextureAlphaPreMultiplied:
        case TextureBlendOperation::BlendCurrentAlpha:
        {
            DXSetShaders(VertexShaderName::Dirlight1, PixelShaderName::Decal);
            break;
        }
        case TextureBlendOperation::Add:
        {
            DXSetShaders(VertexShaderName::Vgloss, PixelShaderName::Pgloss);
            break;
        }
        case TextureBlendOperation::AddSigned:
        {
            DXSetShaders(VertexShaderName::Shdwbkfc1, PixelShaderName::Decalkil);
            break;
        }
        case TextureBlendOperation::Subtract:
        {
            DXSetShaders(VertexShaderName::Vomni1, PixelShaderName::Decal);
            break;
        }
        case TextureBlendOperation::AddSmooth:
        {
            DXSetShaders(VertexShaderName::Vspecmap, PixelShaderName::Decal);
            break;
        }
        default:
        {
            DXSetShaders(VertexShaderName::Vlmap2, PixelShaderName::Plmap);
            break;
        }
    }

    DXSetVertexShaderValues(0);
}

// todo rename
void DXSetShadersMode(const u32 mode)
{
    switch (State.DX.Textures.BlendOperation)
    {
        case TextureBlendOperation::None:
        {
            DXSetShaders(VertexShaderName::Dirlight2, PixelShaderName::Decal);
            break;
        }
        case TextureBlendOperation::Disable:
        {
            if (mode & GRAPHICS_MODE_UNKNOWN_400)
            {
                DXSetShaders(VertexShaderName::Vbumpgen, PixelShaderName::Bump);
            }
            else
            {
                DXSetShaders(VertexShaderName::Vbump, PixelShaderName::Bump);
            }

            break;
        }
        case TextureBlendOperation::SelectArgument1:
        {
            if (mode & GRAPHICS_MODE_UNKNOWN_400)
            {
                DXSetShaders(VertexShaderName::Vbmpspgn, PixelShaderName::Bumpspec);
            }
            else
            {
                DXSetShaders(VertexShaderName::Vbmpspec, PixelShaderName::Bumpspec);
            }

            break;
        }
        case TextureBlendOperation::Modulate:
        {
            DXSetShaders(VertexShaderName::Vbmpcube, PixelShaderName::Pbmpcube);
            break;
        }
        case TextureBlendOperation::AddSigned:
        {
            DXSetShaders(VertexShaderName::Shdwbkfc2, PixelShaderName::Decalkil);
            break;
        }
        case TextureBlendOperation::AddSigned2X:
        {
            DXSetShaders(VertexShaderName::Vlmap1, PixelShaderName::Plmap);
            break;
        }
        case TextureBlendOperation::Subtract:
        {
            DXSetShaders(VertexShaderName::Vomni3, PixelShaderName::Decal);
            break;
        }
        case TextureBlendOperation::BlendDiffuseAlpha:
        {
            DXSetShaders(VertexShaderName::Vbmpspom, PixelShaderName::Pbmpsplm);
            break;
        }
        case TextureBlendOperation::BlendTextureAlpha:
        {
            DXSetShaders(VertexShaderName::Vomnibmp, PixelShaderName::Bump);
            break;
        }
        case TextureBlendOperation::BlendFactorAlpha:
        {
            DXSetShaders(VertexShaderName::Vbmpspom, PixelShaderName::Bumpspec);
            break;
        }
        case TextureBlendOperation::PreModulate:
        {
            DXSetShaders(VertexShaderName::Vlmbs, PixelShaderName::Plmbs);
            break;
        }
        default:
        {
            DXSetShaders((VertexShaderName)mode, (PixelShaderName)mode);
            break;
        }
    }

    DXSetVertexShaderValues(1);
}

// todo: rename parameter
void DXSetVertexShaderValues(const u32 mode)
{
    if (State.Settings.UseFixedFunctionPipe != 0) { return; }

    if (State.DX.Textures.BlendOperation == TextureBlendOperation::Disable)
    {
        // todo: I think this is in reverse order (values inside constant)
        struct Vector4 constants[] =
        {
            {
                0.0f,
                State.DX.Light.Colors.B[0],
                State.DX.Light.Colors.G[0],
                State.DX.Light.Colors.R[0]
            },
            {
                0.0f,
                State.DX.Light.Lights[0].Ambient.b,
                State.DX.Light.Lights[0].Ambient.g,
                State.DX.Light.Lights[0].Ambient.r
            }
        };

        DXC(State.DX.DirectXDevice->SetPixelShaderConstant(0, &constants, ARRAYSIZE(constants)), "Unable to set pixel shader constants.");
    }

    if (!State.DX.Light.IsChanged) { return; }

    State.DX.Light.IsChanged = FALSE;

    struct Matrix4x4 matrix;

    MatrixMultiply(&matrix, &State.DX.Transform.Matrix1, &State.DX.Transform.Projection);

    State.DX.Shaders.Constants.Staging[0].X = matrix._11;
    State.DX.Shaders.Constants.Staging[0].Y = matrix._21;
    State.DX.Shaders.Constants.Staging[0].Z = matrix._31;
    State.DX.Shaders.Constants.Staging[0].W = matrix._41;

    State.DX.Shaders.Constants.Staging[1].X = matrix._12;
    State.DX.Shaders.Constants.Staging[1].Y = matrix._22;
    State.DX.Shaders.Constants.Staging[1].Z = matrix._32;
    State.DX.Shaders.Constants.Staging[1].W = matrix._42;

    State.DX.Shaders.Constants.Staging[2].X = matrix._13;
    State.DX.Shaders.Constants.Staging[2].Y = matrix._23;
    State.DX.Shaders.Constants.Staging[2].Z = matrix._33;
    State.DX.Shaders.Constants.Staging[2].W = matrix._43;

    State.DX.Shaders.Constants.Staging[3].X = matrix._14;
    State.DX.Shaders.Constants.Staging[3].Y = matrix._24;
    State.DX.Shaders.Constants.Staging[3].Z = matrix._34;
    State.DX.Shaders.Constants.Staging[3].W = matrix._44;

    State.DX.Shaders.Constants.Staging[7].X = 1.0f;
    State.DX.Shaders.Constants.Staging[7].Y = 1.0f;
    State.DX.Shaders.Constants.Staging[7].Z = 1.0f;
    State.DX.Shaders.Constants.Staging[7].W = 1.0f;

    State.DX.Shaders.Constants.Staging[24].X = 0.0f;
    State.DX.Shaders.Constants.Staging[24].Y = 0.0f;
    State.DX.Shaders.Constants.Staging[24].Z = 0.0f;
    State.DX.Shaders.Constants.Staging[24].W = 0.0f;

    if (State.DX.Light.IsEnabled)
    {
        State.DX.Shaders.Constants.Staging[4].X = -State.DX.Light.XYZ[0].X;
        State.DX.Shaders.Constants.Staging[4].Y = -State.DX.Light.XYZ[0].Y;
        State.DX.Shaders.Constants.Staging[4].Z = -State.DX.Light.XYZ[0].Z;

        State.DX.Shaders.Constants.Staging[8].X = State.DX.Light.Colors.R[0];
        State.DX.Shaders.Constants.Staging[8].Y = State.DX.Light.Colors.R[1];
        State.DX.Shaders.Constants.Staging[8].Z = State.DX.Light.Colors.R[2];
        State.DX.Shaders.Constants.Staging[8].W = State.DX.Light.Lights[0].Ambient.r;

        State.DX.Shaders.Constants.Staging[20].X = -State.DX.Light.XYZ[1].X;
        State.DX.Shaders.Constants.Staging[20].Y = -State.DX.Light.XYZ[1].Y;
        State.DX.Shaders.Constants.Staging[20].Z = -State.DX.Light.XYZ[1].Z;

        State.DX.Shaders.Constants.Staging[21].X = State.DX.Light.Colors.G[0];
        State.DX.Shaders.Constants.Staging[21].Y = State.DX.Light.Colors.G[1];
        State.DX.Shaders.Constants.Staging[21].Z = State.DX.Light.Colors.G[2];
        State.DX.Shaders.Constants.Staging[21].W = State.DX.Light.Lights[0].Ambient.g;

        State.DX.Shaders.Constants.Staging[22].X = -State.DX.Light.XYZ[2].X;
        State.DX.Shaders.Constants.Staging[22].Y = -State.DX.Light.XYZ[2].Y;
        State.DX.Shaders.Constants.Staging[22].Z = -State.DX.Light.XYZ[2].Z;

        State.DX.Shaders.Constants.Staging[23].X = State.DX.Light.Colors.B[0];
        State.DX.Shaders.Constants.Staging[23].Y = State.DX.Light.Colors.B[1];
        State.DX.Shaders.Constants.Staging[23].Z = State.DX.Light.Colors.B[2];
        State.DX.Shaders.Constants.Staging[23].W = State.DX.Light.Lights[0].Ambient.b;
    }
    else
    {
        State.DX.Shaders.Constants.Staging[4].X = 0.0f;
        State.DX.Shaders.Constants.Staging[4].Y = 0.0f;
        State.DX.Shaders.Constants.Staging[4].Z = 0.0f;

        State.DX.Shaders.Constants.Staging[8].X = 0.0f;
        State.DX.Shaders.Constants.Staging[8].Y = 0.0f;
        State.DX.Shaders.Constants.Staging[8].Z = 0.0f;
        State.DX.Shaders.Constants.Staging[8].W = 1.0f;

        State.DX.Shaders.Constants.Staging[20].X = 0.0f;
        State.DX.Shaders.Constants.Staging[20].Y = 0.0f;
        State.DX.Shaders.Constants.Staging[20].Z = 0.0f;

        State.DX.Shaders.Constants.Staging[21].X = 0.0f;
        State.DX.Shaders.Constants.Staging[21].Y = 0.0f;
        State.DX.Shaders.Constants.Staging[21].Z = 0.0f;
        State.DX.Shaders.Constants.Staging[21].W = 1.0f;

        State.DX.Shaders.Constants.Staging[22].X = 0.0f;
        State.DX.Shaders.Constants.Staging[22].Y = 0.0f;
        State.DX.Shaders.Constants.Staging[22].Z = 0.0f;

        State.DX.Shaders.Constants.Staging[23].X = 0.0f;
        State.DX.Shaders.Constants.Staging[23].Y = 0.0f;
        State.DX.Shaders.Constants.Staging[23].Z = 0.0f;
        State.DX.Shaders.Constants.Staging[23].W = 1.0f;
    }

    State.DX.Shaders.Constants.Staging[4].W = 0.0f;

    State.DX.Shaders.Constants.Staging[5].X = 0.0f;
    State.DX.Shaders.Constants.Staging[5].Y = 1.0f;
    State.DX.Shaders.Constants.Staging[5].Z = 0.5f;
    State.DX.Shaders.Constants.Staging[5].W = State.DX.Light.Alpha;

    State.DX.Shaders.Constants.Staging[6].X = State.DX.FogStart;
    State.DX.Shaders.Constants.Staging[6].Y = 1.0f / (State.DX.FogEnd - State.DX.FogStart);

    State.DX.Shaders.Constants.Staging[9].X = State.DX.Transform.UnknownFloats4[0];
    State.DX.Shaders.Constants.Staging[9].Y = State.DX.Transform.UnknownFloats4[1];
    State.DX.Shaders.Constants.Staging[9].Z = State.DX.Transform.UnknownFloats4[2];
    State.DX.Shaders.Constants.Staging[9].W = 0.0f;

    State.DX.Shaders.Constants.Staging[10].X = State.DX.FogR;
    State.DX.Shaders.Constants.Staging[10].Y = State.DX.FogG;
    State.DX.Shaders.Constants.Staging[10].Z = State.DX.FogB;
    State.DX.Shaders.Constants.Staging[10].W = 0.0f;

    State.DX.Shaders.Constants.Staging[12].X = State.DX.Transform.O2W._11;
    State.DX.Shaders.Constants.Staging[12].Y = State.DX.Transform.O2W._21;
    State.DX.Shaders.Constants.Staging[12].Z = State.DX.Transform.O2W._31;
    State.DX.Shaders.Constants.Staging[12].W = State.DX.Transform.O2W._41;

    State.DX.Shaders.Constants.Staging[13].X = State.DX.Transform.O2W._12;
    State.DX.Shaders.Constants.Staging[13].Y = State.DX.Transform.O2W._22;
    State.DX.Shaders.Constants.Staging[13].Z = State.DX.Transform.O2W._32;
    State.DX.Shaders.Constants.Staging[13].W = State.DX.Transform.O2W._42;

    State.DX.Shaders.Constants.Staging[14].X = State.DX.Transform.O2W._13;
    State.DX.Shaders.Constants.Staging[14].Y = State.DX.Transform.O2W._23;
    State.DX.Shaders.Constants.Staging[14].Z = State.DX.Transform.O2W._33;
    State.DX.Shaders.Constants.Staging[14].W = State.DX.Transform.O2W._43;

    State.DX.Shaders.Constants.Staging[20].W = 0.0f;

    State.DX.Shaders.Constants.Staging[22].W = 0.0f;

    if (State.DX.Mode.Mode & GRAPHICS_MODE_UNKNOWN_400)
    {
        State.DX.Shaders.Constants.Staging[15].X = State.DX.Transform.W2L._11;
        State.DX.Shaders.Constants.Staging[15].Y = State.DX.Transform.W2L._21;
        State.DX.Shaders.Constants.Staging[15].Z = State.DX.Transform.W2L._31;
        State.DX.Shaders.Constants.Staging[15].W = State.DX.Transform.W2L._41;

        State.DX.Shaders.Constants.Staging[16].X = State.DX.Transform.W2L._12;
        State.DX.Shaders.Constants.Staging[16].Y = State.DX.Transform.W2L._22;
        State.DX.Shaders.Constants.Staging[16].Z = State.DX.Transform.W2L._32;
        State.DX.Shaders.Constants.Staging[16].W = State.DX.Transform.W2L._42;

        State.DX.Shaders.Constants.Staging[17].X = State.DX.Transform.W2L._13;
        State.DX.Shaders.Constants.Staging[17].Y = State.DX.Transform.W2L._23;
        State.DX.Shaders.Constants.Staging[17].Z = State.DX.Transform.W2L._33;
        State.DX.Shaders.Constants.Staging[17].W = State.DX.Transform.W2L._43;

        State.DX.Shaders.Constants.Staging[18].X = State.DX.Transform.W2L._14;
        State.DX.Shaders.Constants.Staging[18].Y = State.DX.Transform.W2L._24;
        State.DX.Shaders.Constants.Staging[18].Z = State.DX.Transform.W2L._34;
        State.DX.Shaders.Constants.Staging[18].W = State.DX.Transform.W2L._44;
    }

    for (auto x = 0; x < 25; x++) // todo: why ( x is register)
    {
        auto actual = &State.DX.Shaders.Constants.Actual[x];
        auto staging = &State.DX.Shaders.Constants.Staging[x];

        if (actual->X != staging->X || actual->Y != staging->Y || actual->Z != staging->Z || actual->W != staging->W)
        {
            DXC(State.DX.DirectXDevice->SetVertexShaderConstant(x, &staging, 1), "Unable to set vertex shader constants.");

            actual->X = staging->X;
            actual->Y = staging->Y;
            actual->Z = staging->Z;
            actual->W = staging->W;
        }
    }
}

// todo: rename
void DXSetTLShaders(u32 mode)
{
    switch (State.DX.Textures.BlendOperation)
    {
        case TextureBlendOperation::AddSigned:
        {
            DXSetShaders(VertexShaderName::Shdwbkfc1, PixelShaderName::Decalkil);
            break;
        }
        case TextureBlendOperation::Subtract:
        {
            DXSetShaders(VertexShaderName::Vomni2, PixelShaderName::Decal);
            break;
        }
        default:
        {
            if (mode & GRAPHICS_MODE_UNKNOWN_400)
            {
                DXSetShaders(VertexShaderName::Pretex, PixelShaderName::Decalkil);
            }
            else
            {
                DXSetShaders(VertexShaderName::Prelit2, PixelShaderName::Decal);
            }

            break;
        }
    }

    DXSetVertexShaderValues(0);
}

u32 DXSetLight(void)
{
    State.DX.Light.IsChanged = TRUE;

    if (State.Settings.UseFixedFunctionPipe == 0) { return TRUE; }

    State.DX.Light.Lights[0].Type = D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL;
    State.DX.Light.Lights[0].Diffuse.a = 1.0f;
    State.DX.Light.Lights[0].Specular.r = 0.0f;
    State.DX.Light.Lights[0].Specular.g = 0.0f;
    State.DX.Light.Lights[0].Specular.b = 0.0f;
    State.DX.Light.Lights[0].Specular.a = 0.0f;
    State.DX.Light.Lights[0].Position.x = 0.0f;
    State.DX.Light.Lights[0].Position.y = 0.0f;
    State.DX.Light.Lights[0].Position.z = 0.0f;
    State.DX.Light.Lights[0].Range = 0.0f;
    State.DX.Light.Lights[0].Falloff = 0.0f;
    State.DX.Light.Lights[0].Attenuation0 = 0.0f;
    State.DX.Light.Lights[0].Attenuation1 = 0.0f;
    State.DX.Light.Lights[0].Attenuation2 = 0.0f;
    State.DX.Light.Lights[0].Theta = 0.0f;
    State.DX.Light.Lights[0].Phi = 0.0f;

    State.DX.Light.Lights[1].Type = D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL;
    State.DX.Light.Lights[1].Diffuse.a = 0.0f;
    State.DX.Light.Lights[1].Specular.r = 0.0f;
    State.DX.Light.Lights[1].Specular.g = 0.0f;
    State.DX.Light.Lights[1].Specular.b = 0.0f;
    State.DX.Light.Lights[1].Specular.a = 0.0f;
    State.DX.Light.Lights[1].Ambient.r = 0.0f;
    State.DX.Light.Lights[1].Ambient.g = 0.0f;
    State.DX.Light.Lights[1].Ambient.b = 0.0f;
    State.DX.Light.Lights[1].Ambient.a = 0.0f;
    State.DX.Light.Lights[1].Position.x = 0.0f;
    State.DX.Light.Lights[1].Position.y = 0.0f;
    State.DX.Light.Lights[1].Position.z = 0.0f;
    State.DX.Light.Lights[1].Range = 0.0f;
    State.DX.Light.Lights[1].Falloff = 0.0f;
    State.DX.Light.Lights[1].Attenuation0 = 0.0f;
    State.DX.Light.Lights[1].Attenuation1 = 0.0f;
    State.DX.Light.Lights[1].Attenuation2 = 0.0f;
    State.DX.Light.Lights[1].Theta = 0.0f;
    State.DX.Light.Lights[1].Phi = 0.0f;

    State.DX.Light.Lights[2].Type = D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL;
    State.DX.Light.Lights[2].Diffuse.a = 0.0f;
    State.DX.Light.Lights[2].Specular.r = 0.0f;
    State.DX.Light.Lights[2].Specular.g = 0.0f;
    State.DX.Light.Lights[2].Specular.b = 0.0f;
    State.DX.Light.Lights[2].Specular.a = 0.0f;
    State.DX.Light.Lights[2].Ambient.r = 0.0f;
    State.DX.Light.Lights[2].Ambient.g = 0.0f;
    State.DX.Light.Lights[2].Ambient.b = 0.0f;
    State.DX.Light.Lights[2].Ambient.a = 0.0f;
    State.DX.Light.Lights[2].Position.x = 0.0f;
    State.DX.Light.Lights[2].Position.y = 0.0f;
    State.DX.Light.Lights[2].Position.z = 0.0f;
    State.DX.Light.Lights[2].Range = 0.0f;
    State.DX.Light.Lights[2].Falloff = 0.0f;
    State.DX.Light.Lights[2].Attenuation0 = 0.0f;
    State.DX.Light.Lights[2].Attenuation1 = 0.0f;
    State.DX.Light.Lights[2].Attenuation2 = 0.0f;
    State.DX.Light.Lights[2].Theta = 0.0f;
    State.DX.Light.Lights[2].Phi = 0.0f;

    for (auto x = 0; x < MAX_LIGHT_COUNT; x++)
    {
        auto light = &State.DX.Light.Lights[x];

        auto isColorMissing = light->Diffuse.r == 0.0f && light->Diffuse.g == 0.0f && light->Diffuse.b == 0.0f;
        auto isDirectionMissing = light->Direction.x == 0.0f && light->Direction.y == 0.0f && light->Direction.z == 0.0f;

        if (isColorMissing || isDirectionMissing)
        {
            State.DX.DirectXDevice->LightEnable(x, FALSE);
        }
        else
        {
            DXC(State.DX.DirectXDevice->SetLight(x, light), "Unable to set light.");
            State.DX.DirectXDevice->LightEnable(x, TRUE);
        }
    }

    return TRUE;
}