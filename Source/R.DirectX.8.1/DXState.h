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
#include "DX.h"
#include "DXPixelShaderConstants.h"
#include "DXShader.h"
#include "DXTexture.h"
#include "DXVertexShaderConstants.h"
#include "Graphics/Color.h"
#include "Graphics/Graphics.h"
#include "Graphics/Texture.h"
#include "Graphics/Vertex.h"
#include "Mathematics/Matrix.h"
#include "Mathematics/Vector.h"

#define MAX_CUBE_TEXTURE_COUNT 100
#define MAX_PALETTE_BUFFER_SIZE 256
#define MAX_PIXEL_BUFFER_SIZE (1024 * 1024 + 512 * 512 + 256 * 256 + 128 * 128 + 64 * 64 + 32 * 32 + 16 * 16 + 8 * 8 + 4 * 4 + 2 * 2 + 1 * 1)
#define MAX_PIXEL_SHADER_COUNT 9
#define MAX_RENDER_TEXTURE_COUNT 100
#define MAX_SELECTED_TEXTURE_COUNT 8
#define MAX_TEXTURE_COUNT 4096
#define MAX_TEXTURE_STAGE_COUNT 256
#define MAX_VERTEX_SHADER_COUNT 33
#define MAX_WINDOW_COUNT 6

namespace Renderer
{
    namespace External
    {
        namespace DX
        {
            struct DXDevice
            {
                s32 Index;

                u32 DeviceID;
                u32 SubSystemID;

                char Driver[MAX_DEVICE_IDENTIFIER_STRING];
                char Description[MAX_DEVICE_IDENTIFIER_STRING];
            };

            struct DXDeviceCapabilities
            {
                D3DCAPS8 DisplayCapabilities;
                D3DPRESENT_PARAMETERS DevicePresentParameters;
                D3DPRESENT_PARAMETERS SwapChainPresentParameters;
            };

            struct DXCurrentDevice
            {
                u32 Index;
                struct DXDeviceCapabilities Capabilities;
            };

            struct DXState
            {
                IDirect3D8* DirectX;
                IDirect3DDevice8* DirectXDevice;

                struct DXCurrentDevice CurrentDevice;
                struct
                {
                    s32 Count;
                    struct DXDevice Details[MAX_DEVICE_COUNT];
                } AvailableDevices;

                struct
                {
                    struct DX::Window::DXWindow* Current;
                    struct DX::Window::DXWindow* Windows[MAX_WINDOW_COUNT];
                } Windows;

                struct
                {
                    // todo: name, type (enum) + options
                    u32 Mode;

                    D3DCULL Cull = D3DCULL::D3DCULL_CCW;

                    struct
                    {
                        f32 Near = 1.0f;
                        f32 Far = 6400.0f;
                    } Clipping;

                    struct
                    {
                        D3DBLEND SourceBlend = D3DBLEND::D3DBLEND_SRCALPHA;
                        D3DBLEND DestinationBlend = D3DBLEND::D3DBLEND_INVSRCALPHA;
                        D3DBLENDOP DestinationBlendOperation = D3DBLENDOP::D3DBLENDOP_ADD;
                    } Blend;
                } Mode;

                BOOL IsStencilSet;
                BOOL IsFrameLocked;
                BOOL IsSceneActive;
                BOOL IsViewportSet = TRUE;

                struct
                {
                    BOOL IsActive = TRUE;
                    BOOL IsChanged = TRUE;

                    f32 Alpha;

                    struct
                    {
                        f32 R[MAX_LIGHT_COUNT] = { 1.0f, 0.0f, 0.0f };
                        f32 G[MAX_LIGHT_COUNT] = { 1.0f, 0.0f, 0.0f };
                        f32 B[MAX_LIGHT_COUNT] = { 1.0f, 0.0f, 0.0f };
                    } Colors;

                    D3DLIGHT8 Lights[MAX_LIGHT_COUNT];

                    struct Renderer::Graphics::RGB Color;
                    struct Mathematics::Vector3 XYZ[MAX_LIGHT_COUNT];
                } Light;

                // todo: Fog structure
                f32 FogR;
                f32 FogG;
                f32 FogB;
                f32 FogStart;
                f32 FogEnd;

                struct
                {
                    D3DFORMAT AlphaSourceSurfaceFormat;
                    Renderer::Graphics::TextureFormat DrawSurfaceFormat;

                    IDirect3DSurface8* DrawSurface;
                    IDirect3DSurface8* DepthSurface;

                    IDirect3DSurface8* AlphaSourceSurface;
                    IDirect3DSurface8* Source565Surface;

                    IDirect3DSurface8* CubeTextureDepthSurface;
                    IDirect3DSurface8* RenderTextureDepthSurface;
                } Surfaces;

                struct
                {
                    IDirect3DIndexBuffer8* IndexBuffer;

                    // todo: names
                    IDirect3DVertexBuffer8* SVertexBuffer;
                    IDirect3DVertexBuffer8* D3DLVertexBuffer;
                    IDirect3DVertexBuffer8* D3DTLVertexBuffer;
                    IDirect3DVertexBuffer8* SVertexBasisBuffer;
                } Buffers;

                struct
                {
                    f32 Near;
                    f32 Far;

                    //todo rename: ModelView
                    struct Mathematics::Matrix4x4 Matrix1;

                    /// <summary>
                    /// Proection matrix.
                    /// </summary>
                    struct Mathematics::Matrix4x4 Projection;

                    /// <summary>
                    /// Object to World matrix.
                    /// </summary>
                    struct Mathematics::Matrix4x4 O2W;

                    /// <summary>
                    /// World to Line Segment matrix.
                    /// </summary>
                    struct Mathematics::Matrix4x4 W2L;

                    /// <summary>
                    /// Identity matrix.
                    /// </summary>
                    const struct Mathematics::Matrix4x4 Identity
                    {
                        1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f
                    };
                } Transform;

                // todo: better name?
                struct Renderer::Graphics::D3DTLVertex Vertexes[10000];
                struct Renderer::Graphics::D3DTLVertex VertexesDL[1000];
                u16 IndexesDL[1000];

                // todo: better name? RenderState?
                s32 State[256];

                struct
                {
                    struct
                    {
                        /// <summary>
                        /// Texture 32-bit ARGB pixel colors palette.
                        /// Note: used during the texture uploading, and is being overwritten each time new texture is uploaded.
                        /// </summary>
                        u32 Colors[MAX_PALETTE_BUFFER_SIZE];

                        /// <summary>
                        /// Texture 32-bit ARGB pixel colors for all mip levels.
                        /// Note: used during the texture uploading, and is being overwritten each time new texture is uploaded.
                        /// </summary>
                        u32 Pixels[MAX_PIXEL_BUFFER_SIZE];
                    } Buffers;

                    struct DXTexture Textures[MAX_TEXTURE_COUNT];

                    // todo: name
                    IDirect3DBaseTexture8* UnknownArray[MAX_SELECTED_TEXTURE_COUNT];

                    Renderer::Graphics::TextureBlendOperation ClampOperation = Renderer::Graphics::TextureBlendOperation::Negative;
                    Renderer::Graphics::TextureBlendOperation BlendOperation;

                    struct
                    {
                        Renderer::Graphics::TextureClamp ClampU[MAX_SELECTED_TEXTURE_COUNT];
                        Renderer::Graphics::TextureClamp ClampV[MAX_SELECTED_TEXTURE_COUNT];

                        IDirect3DBaseTexture8* Textures[MAX_SELECTED_TEXTURE_COUNT];

                        u32 Stages[MAX_SELECTED_TEXTURE_COUNT][MAX_TEXTURE_STAGE_COUNT];
                    } Selected;

                    struct
                    {
                        IDirect3DTexture8* Textures[MAX_RENDER_TEXTURE_COUNT];
                    } Render;

                    struct
                    {
                        D3DCUBEMAP_FACES CurrentSide;
                        IDirect3DCubeTexture8* Textures[MAX_CUBE_TEXTURE_COUNT];
                    } Cube;
                } Textures;

                struct
                {
                    struct
                    {
                        struct Mathematics::Vector4 Actual[25]; // todo: proper size
                        struct Mathematics::Vector4 Staging[25]; // todo: proper size
                    } Constants;

                    struct DX::Shader::DXVertexShader VertexShaders[MAX_VERTEX_SHADER_COUNT]
                    {
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_XYZ_NORMAL_UV,
                            .Function = DX::Shader::Vertex::FUNC_DIRLIGHT,
                            .Name = DX::Shader::Vertex::NAME_DIRLIGHT,
                            .FVF = D3DFVF_TEX2 | D3DFVF_NORMAL | D3DFVF_XYZ,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_XYZ_NORMAL_UV,
                            .Function = DX::Shader::Vertex::FUNC_TEXGEN,
                            .Name = DX::Shader::Vertex::NAME_TEXGEN,
                            .FVF = D3DFVF_TEX2 | D3DFVF_NORMAL | D3DFVF_XYZ,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_XYZ_NORMAL_UV,
                            .Function = DX::Shader::Vertex::FUNC_CUBIC,
                            .Name = DX::Shader::Vertex::NAME_CUBIC,
                            .FVF = D3DFVF_TEX2 | D3DFVF_NORMAL | D3DFVF_XYZ,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_UNKNOWN,
                            .Function = DX::Shader::Vertex::FUNC_PRELIT,
                            .Name = DX::Shader::Vertex::NAME_PRELIT,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_XYZ_DIFFUSE_UV,
                            .Function = DX::Shader::Vertex::FUNC_PRELIT,
                            .Name = DX::Shader::Vertex::NAME_PRELIT,
                            .FVF = D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_UNKNOWN,
                            .Function = DX::Shader::Vertex::FUNC_PRETEX,
                            .Name = DX::Shader::Vertex::NAME_PRETEX,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_BASIS,
                            .Function = DX::Shader::Vertex::FUNC_VBUMP,
                            .Name = DX::Shader::Vertex::NAME_VBUMP,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_BASIS,
                            .Function = DX::Shader::Vertex::FUNC_VBMPSPEC,
                            .Name = DX::Shader::Vertex::NAME_VBMPSPEC,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_BASIS,
                            .Function = DX::Shader::Vertex::FUNC_VBMPCUBE,
                            .Name = DX::Shader::Vertex::NAME_VBMPCUBE,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_BASIS,
                            .Function = DX::Shader::Vertex::FUNC_VBUMPGEN,
                            .Name = DX::Shader::Vertex::NAME_VBUMPGEN,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_BASIS,
                            .Function = DX::Shader::Vertex::FUNC_VBMPSPGN,
                            .Name = DX::Shader::Vertex::NAME_VBMPSPGN,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_BASIS,
                            .Function = DX::Shader::Vertex::FUNC_VBMPSPPL,
                            .Name = DX::Shader::Vertex::NAME_VBMPSPPL,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_BASIS,
                            .Function = DX::Shader::Vertex::FUNC_VBMPSPOM,
                            .Name = DX::Shader::Vertex::NAME_VBMPSPOM,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_XYZ_NORMAL_UV,
                            .Function = DX::Shader::Vertex::FUNC_VGLOSS,
                            .Name = DX::Shader::Vertex::NAME_VGLOSS,
                            .FVF = D3DFVF_TEX2 | D3DFVF_NORMAL | D3DFVF_XYZ,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_BASIS,
                            .Function = DX::Shader::Vertex::FUNC_VLMAP,
                            .Name = DX::Shader::Vertex::NAME_VLMAP,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_XYZ_NORMAL_UV,
                            .Function = DX::Shader::Vertex::FUNC_VLMAP2,
                            .Name = DX::Shader::Vertex::NAME_VLMAP2,
                            .FVF = D3DFVF_TEX2 | D3DFVF_NORMAL | D3DFVF_XYZ,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_SKELETON,
                            .Function = DX::Shader::Vertex::FUNC_VSKEL,
                            .Name = DX::Shader::Vertex::NAME_VSKEL,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_SKELETON,
                            .Function = DX::Shader::Vertex::FUNC_VSKELTEX,
                            .Name = DX::Shader::Vertex::NAME_VSKELTEX,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_UNKNOWN,
                            .Function = DX::Shader::Vertex::FUNC_VOMNI,
                            .Name = DX::Shader::Vertex::NAME_VOMNI,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_XYZ_DIFFUSE_UV,
                            .Function = DX::Shader::Vertex::FUNC_VOMNI,
                            .Name = DX::Shader::Vertex::NAME_VOMNI,
                            .FVF = D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_XYZ_NORMAL_UV,
                            .Function = DX::Shader::Vertex::FUNC_VOMNIN,
                            .Name = DX::Shader::Vertex::NAME_VOMNIN,
                            .FVF = D3DFVF_TEX2 | D3DFVF_NORMAL | D3DFVF_XYZ,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_BASIS,
                            .Function = DX::Shader::Vertex::FUNC_VOMNIBMP,
                            .Name = DX::Shader::Vertex::NAME_VOMNIBMP,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_XYZRHW_DIFFUSE_SPECULAR_UV,
                            .Function = NULL,
                            .Name = DX::Shader::Vertex::NAME_TLVERTEX,
                            .FVF = D3DFVF_TEX1 | D3DFVF_SPECULAR | D3DFVF_DIFFUSE | D3DFVF_XYZRHW,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_XYZ_NORMAL_UV,
                            .Function = DX::Shader::Vertex::FUNC_SHDWBKFC,
                            .Name = DX::Shader::Vertex::NAME_SHDWBKFC,
                            .FVF = D3DFVF_TEX2 | D3DFVF_NORMAL | D3DFVF_XYZ,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_XYZ_NORMAL_UV,
                            .Function = DX::Shader::Vertex::FUNC_VSPECMAP,
                            .Name = DX::Shader::Vertex::NAME_VSPECMAP,
                            .FVF = D3DFVF_TEX2 | D3DFVF_NORMAL | D3DFVF_XYZ,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_SKELETON,
                            .Function = DX::Shader::Vertex::FUNC_VSKELSPC,
                            .Name = DX::Shader::Vertex::NAME_VSKELSPC,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_SKELETON,
                            .Function = DX::Shader::Vertex::FUNC_VSKELSPC2,
                            .Name = DX::Shader::Vertex::NAME_VSKELSPC2,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_SKELETON,
                            .Function = DX::Shader::Vertex::FUNC_VSKELSPC3,
                            .Name = DX::Shader::Vertex::NAME_VSKELSPC3,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_SKELETON,
                            .Function = DX::Shader::Vertex::FUNC_VSKELBMP,
                            .Name = DX::Shader::Vertex::NAME_VSKELBMP,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_BASIS,
                            .Function = DX::Shader::Vertex::FUNC_VOMNI,
                            .Name = DX::Shader::Vertex::NAME_VOMNI,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_BASIS,
                            .Function = DX::Shader::Vertex::FUNC_DIRLIGHT,
                            .Name = DX::Shader::Vertex::NAME_DIRLIGHT,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_BASIS,
                            .Function = DX::Shader::Vertex::FUNC_SHDWBKFC,
                            .Name = DX::Shader::Vertex::NAME_SHDWBKFC,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        },
                        {
                            .Declaration = DX::Shader::Vertex::DECLARATION_BASIS,
                            .Function = DX::Shader::Vertex::FUNC_VLMBS,
                            .Name = DX::Shader::Vertex::NAME_VLMBS,
                            .FVF = D3DFVF_TEX0,
                            .Handle = 0
                        }
                    };

                    struct DX::Shader::DXPixelShader PixelShaders[MAX_PIXEL_SHADER_COUNT]
                    {
                        {
                            .Function = DX::Shader::Pixel::FUNC_DECAL,
                            .Name = DX::Shader::Pixel::NAME_DECAL,
                            .Handle = 0
                        },
                        {
                            .Function = DX::Shader::Pixel::FUNC_DECALKIL,
                            .Name = DX::Shader::Pixel::NAME_DECALKIL,
                            .Handle = 0
                        },
                        {
                            .Function = DX::Shader::Pixel::FUNC_BUMP,
                            .Name = DX::Shader::Pixel::NAME_BUMP,
                            .Handle = 0
                        },
                        {
                            .Function = DX::Shader::Pixel::FUNC_BUMPSPEC,
                            .Name = DX::Shader::Pixel::NAME_BUMPSPEC,
                            .Handle = 0
                        },
                        {
                            .Function = DX::Shader::Pixel::FUNC_PBMPSPLM,
                            .Name = DX::Shader::Pixel::NAME_PBMPSPLM,
                            .Handle = 0
                        },
                        {
                            .Function = DX::Shader::Pixel::FUNC_PBMPCUBE,
                            .Name = DX::Shader::Pixel::NAME_PBMPCUBE,
                            .Handle = 0
                        },
                        {
                            .Function = DX::Shader::Pixel::FUNC_PGLOSS,
                            .Name = DX::Shader::Pixel::NAME_PGLOSS,
                            .Handle = 0
                        },
                        {
                            .Function = DX::Shader::Pixel::FUNC_PLMAP,
                            .Name = DX::Shader::Pixel::NAME_PLMAP,
                            .Handle = 0
                        },
                        {
                            .Function = DX::Shader::Pixel::FUNC_PLMBS,
                            .Name = DX::Shader::Pixel::NAME_PLMBS,
                            .Handle = 0
                        }
                    };
                } Shaders;

                //todo: rename: DepthBufferType
                D3DZBUFFERTYPE ZBufferType = D3DZBUFFERTYPE::D3DZB_TRUE;

                // todo: proper structure to hold counters? "Counters"?
                // todo : rename
                s32 VertexCount;
                s32 BasisVertexCount;
                s32 BVertexCount;
                // todo: rename
                s32 BasisVertexCount2;
                s32 D3DLVertexCount;

                // todo: rename
                // todo: proper structure to hold counters?
                s32 TriangleCount;
            };
        }
    }
}