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
#include "Exit.h"
#include "Module.h"
#include "State.h"

#define MAX_TEXTURE_SURFACE_SIZE 1024

using namespace Renderer;
using namespace Renderer::Module;
using namespace Renderer::Textures;

using namespace Mathematics;
using namespace Renderer::Enums;

namespace DX
{
    u32 Alpha(const f32 a)
    {
        return ((u32)roundf(255.0f * a)) << 24;
    }

    u32 Color(const f32 b, const f32 g, const f32 r, const f32 a)
    {
        auto rc = ((u32)roundf(255.0f * b) && 0xff) << 16;
        auto gc = ((u32)roundf(255.0f * g) && 0xff) << 8;
        auto bc = (u32)roundf(255.0f * r) && 0xff;
        auto ac = (u32)roundf(255.0f * a) << 24;

        return ac | gc | gc | ac;
    }

    void Destroy(void)
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

        ZeroMemory(State.DX.Shaders.Constants.Actual, ARRAYSIZE(State.DX.Shaders.Constants.Actual) * sizeof(struct Vector4));
    }

    BOOL InitializeState(void)
    {
        for (auto x = 0; x < ARRAYSIZE(State.DX.State); x++)
        {
            State.DX.State[x] = -1;
        }

        if (State.DX.DirectXDevice->BeginScene()) { return FALSE; }

        SetRenderState(D3DRENDERSTATETYPE::D3DRS_FILLMODE, D3DFILLMODE::D3DFILL_SOLID);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_SHADEMODE, D3DSHADEMODE::D3DSHADE_GOURAUD);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_LASTPIXEL, FALSE);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_ONE);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL::D3DCULL_NONE);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_DITHERENABLE, TRUE);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHABLENDENABLE, FALSE);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGENABLE, FALSE);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGCOLOR, COLOR_BLACK);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_SPECULARENABLE, TRUE);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZVISIBLE, FALSE);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZENABLE, D3DZBUFFERTYPE::D3DZB_FALSE);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZWRITEENABLE, FALSE);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_ALWAYS);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMPFUNC::D3DCMP_GREATER);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_CLIPPING, TRUE);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_LIGHTING, FALSE);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILZFAIL, D3DSTENCILOP::D3DSTENCILOP_KEEP);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILFAIL, D3DSTENCILOP::D3DSTENCILOP_KEEP);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILREF, 1);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILMASK, -1);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILWRITEMASK, -1);

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

    void Create(void)
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

        if (!State.Settings.IsFixedPipelineActive)
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
                DX::Initialize(dxw->Width, dxw->Height, x);
            }
        }

        DXC(State.DX.DirectXDevice->CreateImageSurface(MAX_TEXTURE_SURFACE_SIZE, MAX_TEXTURE_SURFACE_SIZE,
            State.DX.Surfaces.AlphaSourceSurfaceFormat, &State.DX.Surfaces.AlphaSourceSurface),
            "Unable to create alpha source buffer.");

        DXC(State.DX.DirectXDevice->CreateImageSurface(MAX_TEXTURE_SURFACE_SIZE, MAX_TEXTURE_SURFACE_SIZE,
            D3DFORMAT::D3DFMT_R5G6B5, &State.DX.Surfaces.Source565Surface),
            "Unable to create 565 source buffer.");

        // todo: magic numbers below + types
        auto count = State.Settings.IsFixedPipelineActive ? (65000 * 40) : (65000 * 32); // todo sizeof proper type

        DXC(State.DX.DirectXDevice->CreateVertexBuffer(count, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            State.Settings.IsFixedPipelineActive ? (D3DFVF_TEX2 | D3DFVF_NORMAL | D3DFVF_XYZ) : D3DFVF_TEX0,
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

        if (!InitializeState())
        {
            Exit("Unable to set DirectX into a known state.");
        }
    }

    BOOL ResetDevice(const u32 width, const u32 height, const u32 bpp, u32* unk4, struct DX::DXWindow* dxw)
    {
        if (dxw == NULL) { return FALSE; }

        Destroy();

        dxw->Width = width;
        dxw->Height = height;
        dxw->BitsPerPixel = bpp;
        dxw->Unknown4 = unk4;

        // todo memcopy unk4

        DX::Initialize(dxw, &State.DX.CurrentDevice.Capabilities.DevicePresentParameters);

        DXC(State.DX.DirectXDevice->Reset(&State.DX.CurrentDevice.Capabilities.DevicePresentParameters),
            "Unable to reset device.");

        auto cdxw = State.DX.Windows.Current;
        State.DX.Windows.Current = dxw;

        Create();

        State.DX.Windows.Current = cdxw;

        if (!State.Window.IsWindowMode)
        {
            ShowCursor(TRUE);
        }

        Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, COLOR_BLACK);
        Toggle();

        return TRUE;
    }

    void SetRenderState(const D3DRENDERSTATETYPE state, const s32 value)
    {
        if (State.DX.State[state] != value)
        {
            State.DX.State[state] = value;
            State.DX.DirectXDevice->SetRenderState(state, value);
        }
    }

    void SetMode(const u32 mode)
    {
        // todo
        //SetRenderState(D3DRENDERSTATETYPE::D3DRS_SRCBLEND,
        //    mode & GRAPHICS_MODE_IS_ALPHA_BLEND_ACTIVE ? D3DBLEND::D3DBLEND_SRCALPHA : D3DBLEND::D3DBLEND_ONE);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_SRCBLEND, State.DX.Mode.Blend.SourceBlend);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_DESTBLEND, State.DX.Mode.Blend.DestinationBlend);
        SetRenderState(D3DRENDERSTATETYPE::D3DRS_BLENDOP, State.DX.Mode.Blend.DestinationBlendOperation);

        if ((State.DX.Mode.Mode ^ mode) & GRAPHICS_MODE_IS_TEXTURE_BLENDING_ACTIVE)
        {
            if (mode & GRAPHICS_MODE_IS_TEXTURE_BLENDING_ACTIVE)
            {
                State.DX.Textures.UnknownArray[0] = NULL;
            }
            else
            {
                SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_DISABLE);
            }
        }

        s32 indexes[] = { 0, -1, -1, -1 };

        switch (State.DX.Textures.BlendOperation)
        {
        case BlendOperation::Disable:
        case BlendOperation::BlendTextureAlpha:
        {
            indexes[0] = 1;
            indexes[1] = 0;
            break;
        }
        case BlendOperation::SelectArgument1:
        case BlendOperation::BlendFactorAlpha:
        case BlendOperation::BlendTextureAlphaPreMultiplied:
        case BlendOperation::BlendCurrentAlpha:
        {
            indexes[0] = 1;
            indexes[1] = 4;
            break;
        }
        case BlendOperation::SelectArgument2:
        {
            indexes[0] = 2;
            break;
        }
        case BlendOperation::Modulate:
        {
            indexes[0] = 1;
            indexes[3] = 2;
            break;
        }
        case BlendOperation::Add:
        {
            indexes[0] = 0;
            indexes[1] = 1;
            indexes[2] = 2;
            break;
        }
        case BlendOperation::AddSigned2X:
        {
            indexes[0] = 0;
            indexes[1] = 3;
            break;
        }
        case BlendOperation::BlendDiffuseAlpha:
        {
            indexes[0] = 1;
            indexes[1] = 4;
            indexes[2] = 3;
            break;
        }
        case BlendOperation::PreModulate:
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
                    SetTextureStageValue(x, D3DTEXTURESTAGESTATETYPE::D3DTSS_MAGFILTER, D3DTEXTUREFILTERTYPE::D3DTEXF_LINEAR);
                    SetTextureStageValue(x, D3DTEXTURESTAGESTATETYPE::D3DTSS_MINFILTER, D3DTEXTUREFILTERTYPE::D3DTEXF_LINEAR);
                    SetTextureStageValue(x, D3DTEXTURESTAGESTATETYPE::D3DTSS_MIPFILTER, D3DTEXTUREFILTERTYPE::D3DTEXF_LINEAR);

                    auto indx = indexes[x];

                    SetTextureStageValue(x, D3DTEXTURESTAGESTATETYPE::D3DTSS_ADDRESSU,
                        State.DX.Textures.Selected.ClampU[indx] == ClampMode::Wrap
                        ? D3DTEXTUREADDRESS::D3DTADDRESS_WRAP : D3DTEXTUREADDRESS::D3DTADDRESS_CLAMP);
                    SetTextureStageValue(x, D3DTEXTURESTAGESTATETYPE::D3DTSS_ADDRESSV,
                        State.DX.Textures.Selected.ClampV[indx] == ClampMode::Wrap
                        ? D3DTEXTUREADDRESS::D3DTADDRESS_WRAP : D3DTEXTUREADDRESS::D3DTADDRESS_CLAMP);

                    if (!State.Settings.IsFixedPipelineActive)
                    {
                        SetTextureStageValue(x, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXTURETRANSFORMFLAGS,
                            (mode & GRAPHICS_MODE_UNKNOWN_400)
                            ? D3DTEXTURETRANSFORMFLAGS::D3DTTFF_PROJECTED : D3DTEXTURETRANSFORMFLAGS::D3DTTFF_DISABLE);
                    }
                }
            }

            if (State.DX.Textures.BlendOperation == BlendOperation::None)
            {
                SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
                SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_MODULATE);
                SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLORARG1, D3DTA_TEXTURE);
                SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLORARG2, D3DTA_DIFFUSE);
                SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_MODULATE);
                SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
                SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

                SetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_DISABLE);
                SetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_DISABLE);

                SetTextureStageValue(2, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_DISABLE);
                SetTextureStageValue(2, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_DISABLE);

                SetTextureStageValue(3, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_DISABLE);
                SetTextureStageValue(3, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_DISABLE);
            }
            else if (State.DX.Textures.BlendOperation == BlendOperation::AddSigned2X)
            {
                SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
                SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_MODULATE);
                SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLORARG1, D3DTA_TEXTURE);
                SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLORARG2, D3DTA_DIFFUSE);
                SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_MODULATE);
                SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
                SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

                SetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
                SetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_MODULATE);
                SetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLORARG1, D3DTA_TEXTURE);
                SetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLORARG2, D3DTA_CURRENT);
                SetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_MODULATE);
                SetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
                SetTextureStageValue(1, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAARG2, D3DTA_CURRENT);

                SetTextureStageValue(2, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_DISABLE);
                SetTextureStageValue(2, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_DISABLE);

                SetTextureStageValue(3, D3DTEXTURESTAGESTATETYPE::D3DTSS_COLOROP, D3DTEXTUREOP::D3DTOP_DISABLE);
                SetTextureStageValue(3, D3DTEXTURESTAGESTATETYPE::D3DTSS_ALPHAOP, D3DTEXTUREOP::D3DTOP_DISABLE);
            }

            if (State.Settings.IsFixedPipelineActive)
            {
                if (mode & GRAPHICS_MODE_UNKNOWN_400)
                {
                    SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXTURETRANSFORMFLAGS,
                        D3DTEXTURETRANSFORMFLAGS::D3DTTFF_PROJECTED | D3DTEXTURETRANSFORMFLAGS::D3DTTFF_COUNT3);
                    SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
                }
                else
                {
                    if (State.DX.Textures.BlendOperation == BlendOperation::SelectArgument2)
                    {
                        SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXTURETRANSFORMFLAGS, D3DTEXTURETRANSFORMFLAGS::D3DTTFF_COUNT3);
                        SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR);
                    }
                    else
                    {
                        SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXTURETRANSFORMFLAGS, D3DTEXTURETRANSFORMFLAGS::D3DTTFF_DISABLE);
                        SetTextureStageValue(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
                    }
                }
            }
        }

        State.DX.Textures.ClampOperation = State.DX.Textures.BlendOperation;

        if ((State.DX.Mode.Mode ^ mode) & GRAPHICS_MODE_IS_ALPHA_BLEND_ACTIVE)
        {
            if (mode & GRAPHICS_MODE_IS_ALPHA_BLEND_ACTIVE)
            {
                SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHABLENDENABLE, TRUE);
                SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
            }
            else
            {
                SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHABLENDENABLE, FALSE);
                SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE);
            }
        }

        if ((State.DX.Mode.Mode ^ mode) & GRAPHICS_MODE_IS_FOG_ACTIVE)
        {
            if (mode & GRAPHICS_MODE_IS_FOG_ACTIVE)
            {
                SetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGENABLE, TRUE);

                if (State.Settings.IsFixedPipelineActive)
                {
                    SetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGVERTEXMODE, D3DFOGMODE::D3DFOG_LINEAR);
                    SetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGTABLEMODE, D3DFOGMODE::D3DFOG_NONE);
                }
            }
            else
            {
                SetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGENABLE, FALSE);

                if (State.Settings.IsFixedPipelineActive)
                {
                    SetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGVERTEXMODE, D3DFOGMODE::D3DFOG_NONE);
                    SetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGTABLEMODE, D3DFOGMODE::D3DFOG_NONE);
                }
            }
        }

        if (State.DX.IsStencilSet)
        {
            State.DX.Mode.Mode = mode;
            return;
        }

        if (!((State.DX.Mode.Mode ^ mode) & (GRAPHICS_MODE_IS_DEPTH_BUFFER_ACTIVE | GRAPHICS_MODE_IS_DEPTH_BUFFER_WRITES_ACTIVE)))
        {
            State.DX.Mode.Mode = mode;
            return;
        }

        if (mode & (GRAPHICS_MODE_IS_DEPTH_BUFFER_ACTIVE | GRAPHICS_MODE_IS_DEPTH_BUFFER_WRITES_ACTIVE))
        {
            SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZENABLE, State.DX.ZBufferType);

            if ((mode & GRAPHICS_MODE_IS_DEPTH_BUFFER_ACTIVE) && (mode & GRAPHICS_MODE_IS_DEPTH_BUFFER_WRITES_ACTIVE))
            {
                SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZWRITEENABLE, TRUE);
                SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_LESSEQUAL);
            }
            else if (mode & GRAPHICS_MODE_IS_DEPTH_BUFFER_ACTIVE)
            {
                SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZWRITEENABLE, FALSE);
                SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_LESSEQUAL);
            }
            else
            {
                SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZWRITEENABLE, TRUE);
                SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_ALWAYS);
            }
        }
        else
        {
            SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZENABLE, D3DZBUFFERTYPE::D3DZB_FALSE);
            SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZWRITEENABLE, FALSE);
            SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_ALWAYS);
        }

        State.DX.Mode.Mode = mode;
    }

    void SetTextureStageValue(const u32 stage, const D3DTEXTURESTAGESTATETYPE type, const u32 value)
    {
        if (State.DX.Textures.Selected.Stages[stage][type] != value)
        {
            State.DX.DirectXDevice->SetTextureStageState(stage, type, value);
            State.DX.Textures.Selected.Stages[stage][type] = value;
        }
    }

    void SetShaders(const DX::Shaders::DXVertexShaderName vsn, const DX::Shaders::DXPixelShaderName psn)
    {
        auto vs = &State.DX.Shaders.VertexShaders[(s32)vsn];

        DXC(State.DX.DirectXDevice->SetVertexShader(vs->Handle == 0 ? vs->FVF : vs->Handle),
            "Unable to set vertex shader.");

        if (!State.Settings.IsFixedPipelineActive)
        {
            DXC(State.DX.DirectXDevice->SetPixelShader(State.DX.Shaders.PixelShaders[(s32)psn].Handle),
                "Unable to set pixel shader.");
        }
    }

    void SetIndexSource(const u32 index, IDirect3DIndexBuffer8* buffer)
    {
        DXC(State.DX.DirectXDevice->SetIndices(buffer, index), "Unable to set index source.");
    }

    void SetIndexes(const u16* indexes, const u32 count)
    {
        if (65000 < count)
        {
            Exit("Too many indexes.");
        }

        if (65000 < State.DX.TriangleCount + count)
        {
            State.DX.TriangleCount = 0;
        }

        auto options = State.DX.TriangleCount == 0
            ? D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK
            : D3DLOCK_NOOVERWRITE | D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK;

        u16* buffer;
        DXC(State.DX.Buffers.IndexBuffer->Lock(State.DX.TriangleCount * 3 * sizeof(u16),
            count * 3 * sizeof(u16), (BYTE**)&buffer, options), "Unable to lock index buffer.");

        CopyMemory(buffer, indexes, count * 3 * sizeof(u16));

        DXC(State.DX.Buffers.IndexBuffer->Unlock(), "Unable to unlock index buffer.");
    }

    void SetLightShaders(const u32 mode)
    {
        switch (State.DX.Textures.BlendOperation)
        {
        case BlendOperation::Disable:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Prelit1, DX::Shaders::DXPixelShaderName::Bump);
            break;
        }
        case BlendOperation::SelectArgument1:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Prelit1, DX::Shaders::DXPixelShaderName::Bumpspec);
            break;
        }
        case BlendOperation::AddSigned:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Shdwbkfc1, DX::Shaders::DXPixelShaderName::Decalkil);
            break;
        }
        case BlendOperation::Subtract:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vomni1, DX::Shaders::DXPixelShaderName::Decal);
            break;
        }
        default:
        {
            if (mode & GRAPHICS_MODE_UNKNOWN_400)
            {
                SetShaders(DX::Shaders::DXVertexShaderName::Pretex, DX::Shaders::DXPixelShaderName::Decalkil);
            }
            else
            {
                SetShaders(DX::Shaders::DXVertexShaderName::Prelit1, DX::Shaders::DXPixelShaderName::Decal);
            }

            break;
        }
        }

        SetVertexShaderValues(0);
    }

    void SetSkeletonShaders(const u32 mode)
    {
        switch (State.DX.Textures.BlendOperation)
        {
        case BlendOperation::Disable:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vskelbmp, DX::Shaders::DXPixelShaderName::Bump);
            break;
        }
        case BlendOperation::SelectArgument1:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vskelspc, DX::Shaders::DXPixelShaderName::Bumpspec);
            break;
        }
        case BlendOperation::BlendTextureAlphaPreMultiplied:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vskelsp2, DX::Shaders::DXPixelShaderName::Bumpspec);
            break;
        }
        case BlendOperation::BlendCurrentAlpha:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vskelsp3, DX::Shaders::DXPixelShaderName::Bumpspec);
            break;
        }
        default:
        {
            if (mode & GRAPHICS_MODE_UNKNOWN_400)
            {
                SetShaders(DX::Shaders::DXVertexShaderName::Vskeltex, DX::Shaders::DXPixelShaderName::Decalkil);
            }
            else
            {
                SetShaders(DX::Shaders::DXVertexShaderName::Vskel, DX::Shaders::DXPixelShaderName::Decal);
            }

            break;
        }
        }

        SetVertexShaderValues(0);
    }

    // todo: rename
    void SetBlendShaders(const u32 mode)
    {
        switch (State.DX.Textures.BlendOperation)
        {
        case BlendOperation::None:
        {
            if (mode & GRAPHICS_MODE_UNKNOWN_400)
            {
                    SetShaders(DX::Shaders::DXVertexShaderName::Texgen, DX::Shaders::DXPixelShaderName::Decalkil);
            }
            else
            {
                SetShaders(DX::Shaders::DXVertexShaderName::Dirlight1, DX::Shaders::DXPixelShaderName::Decal);
            }

            break;
        }
        case BlendOperation::SelectArgument2:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Cubic, DX::Shaders::DXPixelShaderName::Decal);
            break;
        }
        case BlendOperation::Disable:
        case BlendOperation::SelectArgument1:
        case BlendOperation::Modulate:
        case BlendOperation::BlendDiffuseAlpha:
        case BlendOperation::BlendTextureAlpha:
        case BlendOperation::BlendFactorAlpha:
        case BlendOperation::BlendTextureAlphaPreMultiplied:
        case BlendOperation::BlendCurrentAlpha:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Dirlight1, DX::Shaders::DXPixelShaderName::Decal);
            break;
        }
        case BlendOperation::Add:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vgloss, DX::Shaders::DXPixelShaderName::Pgloss);
            break;
        }
        case BlendOperation::AddSigned:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Shdwbkfc1, DX::Shaders::DXPixelShaderName::Decalkil);
            break;
        }
        case BlendOperation::Subtract:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vomni1, DX::Shaders::DXPixelShaderName::Decal);
            break;
        }
        case BlendOperation::AddSmooth:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vspecmap, DX::Shaders::DXPixelShaderName::Decal);
            break;
        }
        default:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vlmap2, DX::Shaders::DXPixelShaderName::Plmap);
            break;
        }
        }

        SetVertexShaderValues(0);
    }

    // todo rename
    void SetShadersMode(const u32 mode)
    {
        switch (State.DX.Textures.BlendOperation)
        {
        case BlendOperation::None:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Dirlight2, DX::Shaders::DXPixelShaderName::Decal);
            break;
        }
        case BlendOperation::Disable:
        {
            if (mode & GRAPHICS_MODE_UNKNOWN_400)
            {
                SetShaders(DX::Shaders::DXVertexShaderName::Vbumpgen, DX::Shaders::DXPixelShaderName::Bump);
            }
            else
            {
                SetShaders(DX::Shaders::DXVertexShaderName::Vbump, DX::Shaders::DXPixelShaderName::Bump);
            }

            break;
        }
        case BlendOperation::SelectArgument1:
        {
            if (mode & GRAPHICS_MODE_UNKNOWN_400)
            {
                SetShaders(DX::Shaders::DXVertexShaderName::Vbmpspgn, DX::Shaders::DXPixelShaderName::Bumpspec);
            }
            else
            {
                SetShaders(DX::Shaders::DXVertexShaderName::Vbmpspec, DX::Shaders::DXPixelShaderName::Bumpspec);
            }

            break;
        }
        case BlendOperation::Modulate:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vbmpcube, DX::Shaders::DXPixelShaderName::Pbmpcube);
            break;
        }
        case BlendOperation::AddSigned:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Shdwbkfc2, DX::Shaders::DXPixelShaderName::Decalkil);
            break;
        }
        case BlendOperation::AddSigned2X:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vlmap1, DX::Shaders::DXPixelShaderName::Plmap);
            break;
        }
        case BlendOperation::Subtract:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vomni3, DX::Shaders::DXPixelShaderName::Decal);
            break;
        }
        case BlendOperation::BlendDiffuseAlpha:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vbmpspom, DX::Shaders::DXPixelShaderName::Pbmpsplm);
            break;
        }
        case BlendOperation::BlendTextureAlpha:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vomnibmp, DX::Shaders::DXPixelShaderName::Bump);
            break;
        }
        case BlendOperation::BlendFactorAlpha:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vbmpspom, DX::Shaders::DXPixelShaderName::Bumpspec);
            break;
        }
        case BlendOperation::PreModulate:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vlmbs, DX::Shaders::DXPixelShaderName::Plmbs);
            break;
        }
        default:
        {
            SetShaders((DX::Shaders::DXVertexShaderName)mode, (DX::Shaders::DXPixelShaderName)mode);
            break;
        }
        }

        SetVertexShaderValues(1);
    }

    // todo: rename parameter
    void SetVertexShaderValues(const u32 mode)
    {
        if (State.Settings.IsFixedPipelineActive) { return; }

        if (State.DX.Textures.BlendOperation == BlendOperation::Disable)
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

        Multiply(&matrix, &State.DX.Transform.Matrix1, &State.DX.Transform.Projection);

        State.DX.Shaders.Constants.Staging[0].X = matrix.m11;
        State.DX.Shaders.Constants.Staging[0].Y = matrix.m21;
        State.DX.Shaders.Constants.Staging[0].Z = matrix.m31;
        State.DX.Shaders.Constants.Staging[0].W = matrix.m41;

        State.DX.Shaders.Constants.Staging[1].X = matrix.m12;
        State.DX.Shaders.Constants.Staging[1].Y = matrix.m22;
        State.DX.Shaders.Constants.Staging[1].Z = matrix.m32;
        State.DX.Shaders.Constants.Staging[1].W = matrix.m42;

        State.DX.Shaders.Constants.Staging[2].X = matrix.m13;
        State.DX.Shaders.Constants.Staging[2].Y = matrix.m23;
        State.DX.Shaders.Constants.Staging[2].Z = matrix.m33;
        State.DX.Shaders.Constants.Staging[2].W = matrix.m43;

        State.DX.Shaders.Constants.Staging[3].X = matrix.m14;
        State.DX.Shaders.Constants.Staging[3].Y = matrix.m24;
        State.DX.Shaders.Constants.Staging[3].Z = matrix.m34;
        State.DX.Shaders.Constants.Staging[3].W = matrix.m44;

        State.DX.Shaders.Constants.Staging[7].X = 1.0f;
        State.DX.Shaders.Constants.Staging[7].Y = 1.0f;
        State.DX.Shaders.Constants.Staging[7].Z = 1.0f;
        State.DX.Shaders.Constants.Staging[7].W = 1.0f;

        State.DX.Shaders.Constants.Staging[24].X = 0.0f;
        State.DX.Shaders.Constants.Staging[24].Y = 0.0f;
        State.DX.Shaders.Constants.Staging[24].Z = 0.0f;
        State.DX.Shaders.Constants.Staging[24].W = 0.0f;

        if (State.DX.Light.IsActive)
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

        State.DX.Shaders.Constants.Staging[9].X = State.DX.Light.Color.R;
        State.DX.Shaders.Constants.Staging[9].Y = State.DX.Light.Color.G;
        State.DX.Shaders.Constants.Staging[9].Z = State.DX.Light.Color.B;
        State.DX.Shaders.Constants.Staging[9].W = 0.0f;

        State.DX.Shaders.Constants.Staging[10].X = State.DX.FogR;
        State.DX.Shaders.Constants.Staging[10].Y = State.DX.FogG;
        State.DX.Shaders.Constants.Staging[10].Z = State.DX.FogB;
        State.DX.Shaders.Constants.Staging[10].W = 0.0f;

        State.DX.Shaders.Constants.Staging[12].X = State.DX.Transform.O2W.m11;
        State.DX.Shaders.Constants.Staging[12].Y = State.DX.Transform.O2W.m21;
        State.DX.Shaders.Constants.Staging[12].Z = State.DX.Transform.O2W.m31;
        State.DX.Shaders.Constants.Staging[12].W = State.DX.Transform.O2W.m41;

        State.DX.Shaders.Constants.Staging[13].X = State.DX.Transform.O2W.m12;
        State.DX.Shaders.Constants.Staging[13].Y = State.DX.Transform.O2W.m22;
        State.DX.Shaders.Constants.Staging[13].Z = State.DX.Transform.O2W.m32;
        State.DX.Shaders.Constants.Staging[13].W = State.DX.Transform.O2W.m42;

        State.DX.Shaders.Constants.Staging[14].X = State.DX.Transform.O2W.m13;
        State.DX.Shaders.Constants.Staging[14].Y = State.DX.Transform.O2W.m23;
        State.DX.Shaders.Constants.Staging[14].Z = State.DX.Transform.O2W.m33;
        State.DX.Shaders.Constants.Staging[14].W = State.DX.Transform.O2W.m43;

        State.DX.Shaders.Constants.Staging[20].W = 0.0f;

        State.DX.Shaders.Constants.Staging[22].W = 0.0f;

        if (State.DX.Mode.Mode & GRAPHICS_MODE_UNKNOWN_400)
        {
            State.DX.Shaders.Constants.Staging[15].X = State.DX.Transform.W2L.m11;
            State.DX.Shaders.Constants.Staging[15].Y = State.DX.Transform.W2L.m21;
            State.DX.Shaders.Constants.Staging[15].Z = State.DX.Transform.W2L.m31;
            State.DX.Shaders.Constants.Staging[15].W = State.DX.Transform.W2L.m41;

            State.DX.Shaders.Constants.Staging[16].X = State.DX.Transform.W2L.m12;
            State.DX.Shaders.Constants.Staging[16].Y = State.DX.Transform.W2L.m22;
            State.DX.Shaders.Constants.Staging[16].Z = State.DX.Transform.W2L.m32;
            State.DX.Shaders.Constants.Staging[16].W = State.DX.Transform.W2L.m42;

            State.DX.Shaders.Constants.Staging[17].X = State.DX.Transform.W2L.m13;
            State.DX.Shaders.Constants.Staging[17].Y = State.DX.Transform.W2L.m23;
            State.DX.Shaders.Constants.Staging[17].Z = State.DX.Transform.W2L.m33;
            State.DX.Shaders.Constants.Staging[17].W = State.DX.Transform.W2L.m43;

            State.DX.Shaders.Constants.Staging[18].X = State.DX.Transform.W2L.m14;
            State.DX.Shaders.Constants.Staging[18].Y = State.DX.Transform.W2L.m24;
            State.DX.Shaders.Constants.Staging[18].Z = State.DX.Transform.W2L.m34;
            State.DX.Shaders.Constants.Staging[18].W = State.DX.Transform.W2L.m44;
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
    void SetTLShaders(u32 mode)
    {
        switch (State.DX.Textures.BlendOperation)
        {
        case BlendOperation::AddSigned:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Shdwbkfc1, DX::Shaders::DXPixelShaderName::Decalkil);
            break;
        }
        case BlendOperation::Subtract:
        {
            SetShaders(DX::Shaders::DXVertexShaderName::Vomni2, DX::Shaders::DXPixelShaderName::Decal);
            break;
        }
        default:
        {
            if (mode & GRAPHICS_MODE_UNKNOWN_400)
            {
                SetShaders(DX::Shaders::DXVertexShaderName::Pretex, DX::Shaders::DXPixelShaderName::Decalkil);
            }
            else
            {
                SetShaders(DX::Shaders::DXVertexShaderName::Prelit2, DX::Shaders::DXPixelShaderName::Decal);
            }

            break;
        }
        }

        SetVertexShaderValues(0);
    }

    BOOL SetLight(void)
    {
        State.DX.Light.IsChanged = TRUE;

        if (!State.Settings.IsFixedPipelineActive) { return TRUE; }

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
}