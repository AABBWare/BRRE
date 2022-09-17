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
#include "Common.h"
#include "Graphics/Enum.h"
#include "Graphics/Texture.h"
#include "Graphics/Transform.h"
#include "Graphics/Vertex.h"
#include "Win.h"

namespace Renderer
{
    namespace External
    {
        typedef const BOOL(__cdecl* RENDERERALLOCATEOPTIMIZEDMESH) (void);
        typedef const BOOL(__cdecl* RENDERERALLOCATEVERTEXBUFFER) (const void*, const u32, const u32, void**);
        typedef const BOOL(__cdecl* RENDERERBEGINCUBEMAPRENDER) (const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERBEGINRENDERTOTEXTURE) (const u32);
        typedef const BOOL(__cdecl* RENDERERBEGINSCENE) (void);
        typedef const BOOL(__cdecl* RENDERERCREATEGRAPHICSCARDLIST) (s32*, char**, char**, u32*, u32*);
        typedef const BOOL(__cdecl* RENDERERCLEAR) (const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERCLEARZBUFFER) (void);
        typedef const BOOL(__cdecl* RENDERERCOPYBACKBUFFERTORENDERTEXTURE) (void);
        typedef const BOOL(__cdecl* RENDERERDESTROYGAMEWINDOW) (const u32);
        typedef const BOOL(__cdecl* RENDERERDOTLISTD3DL) (const struct Renderer::Graphics::D3DLVertex*, const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERDOTLISTD3DTL) (const struct Renderer::Graphics::D3DTLVertex*, const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERDRAWPIXELS) (void);
        typedef const BOOL(__cdecl* RENDERERENABLECLIPPING) (const Renderer::Graphics::ClippingMode);
        typedef const BOOL(__cdecl* RENDERERENABLECULLING) (const Renderer::Graphics::CullMode);
        typedef const BOOL(__cdecl* RENDERERENABLELIGHTING) (const BOOL);
        typedef const BOOL(__cdecl* RENDERERENDCUBEMAPRENDER) (void);
        typedef const BOOL(__cdecl* RENDERERENDRENDERTOTEXTURE) (void);
        typedef const BOOL(__cdecl* RENDERERENDSCENE) (void);
        typedef const BOOL(__cdecl* RENDERERFREEOPTIMIZEDMESH) (void);
        typedef const BOOL(__cdecl* RENDERERFREETEXTURE) (const u32);
        typedef const BOOL(__cdecl* RENDERERFREEVERTEXBUFFER) (const void*);
        typedef const BOOL(__cdecl* RENDERERGETTEXTUREINFO) (const u32);
        typedef const BOOL(__cdecl* RENDERERHASSHADERSUPPORT) (void);
        typedef const BOOL(__cdecl* RENDERERINIT) (const HWND, const struct InitializeModuleArguments*);
        typedef const BOOL(__cdecl* RENDERERISGAMEWINDOWMODE) (void);
        typedef const BOOL(__cdecl* RENDERERKILL) (void);
        typedef const BOOL(__cdecl* RENDERERLINELISTD3DL) (const struct Renderer::Graphics::D3DLVertex*, const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERLINELISTD3DTL) (const struct Renderer::Graphics::D3DTLVertex*, const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERLOCKFRAME) (void);
        typedef const BOOL(__cdecl* RENDERERLOCKFRAMEFORSCREENSHOT)(void);
        typedef const BOOL(__cdecl* RENDERERPOLYLIST) (const struct Renderer::Graphics::SVertex*, const u32, const u16*, const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERPOLYLISTBASIS) (const struct Renderer::Graphics::SVertexBasis*, const u32, const u16*, const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERPOLYLISTBONE) (const void*, const u32, const u16*, const u32, const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERPOLYLISTD3DL) (const struct Renderer::Graphics::D3DTLVertex*, const u32, const u16*, const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERPOLYLISTD3DTL) (const struct Renderer::Graphics::D3DTLVertex*, const u32, const u16*, const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERPOLYLISTTL) (const struct Renderer::Graphics::TLVertex*, const u32, const u16*, const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERRENDEROPTIMIZEDMESH) (void);
        typedef const BOOL(__cdecl* RENDERERRENDERVERTEXBUFFER) (const void*, const u32, const u16*, const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERRENDERVERTEXBUFFERBASIS) (const void*, const u32, const u16*, const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERRENDERVERTEXBUFFERBONE) (const void*, const u32, const u16*, const u32, const struct Renderer::Graphics::Vector4*, const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERRENDERVERTEXBUFFERPRELIT) (const void*, const u32, const u16*, const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERRESETTEXTURECACHE) (void);
        typedef const BOOL(__cdecl* RENDERERRESTOREVIDEOMODE) (void);
        typedef const BOOL(__cdecl* RENDERERSELECTCUBETEXTURE) (const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERSELECTGAMEWINDOW) (const s32, const s32);
        typedef const BOOL(__cdecl* RENDERERSELECTGRAPHICSCARD) (const u32);
        typedef const BOOL(__cdecl* RENDERERSELECTRENDERTEXTURE) (const u32);
        typedef const BOOL(__cdecl* RENDERERSELECTTEXTURE) (const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERSETALPHA) (const u32);
        typedef const BOOL(__cdecl* RENDERERSETAMBIENTLIGHT) (const f32);
        typedef const BOOL(__cdecl* RENDERERSETAMBIENTLIGHTCOLOR) (const f32, const f32, const f32);
        typedef const BOOL(__cdecl* RENDERERSETCOLORTABLE) (const u8*, u16*);
        typedef const BOOL(__cdecl* RENDERERSETDESTINATIONBLEND) (const Renderer::Graphics::TextureDestinationBlend);
        typedef const BOOL(__cdecl* RENDERERSETDIRTYRECT) (const u32, const u32, const u32, const u32);
        typedef const BOOL(__cdecl* RENDERERSETFOG) (const f32, const f32, const f32, const f32, const f32);
        typedef const BOOL(__cdecl* RENDERERSETGLOSS) (const f32);
        typedef const BOOL(__cdecl* RENDERERSETLIGHTCOLOR) (const f32, const f32, const f32, const s32);
        typedef const BOOL(__cdecl* RENDERERSETLIGHTCONSTANTS) (const f32, const f32, const f32, const f32);
        typedef const BOOL(__cdecl* RENDERERSETLIGHTVECTOR) (void);
        typedef const BOOL(__cdecl* RENDERERSETMULTITEXTUREBLEND) (const Renderer::Graphics::TextureBlendOperation);
        typedef const BOOL(__cdecl* RENDERERSETOMNILIGHT) (const f32*, const f32, const u32);
        typedef const BOOL(__cdecl* RENDERERSETSOURCEBLEND) (const Renderer::Graphics::TextureSourceBlend);
        typedef const BOOL(__cdecl* RENDERERSETSTENCILENABLE) (const BOOL);
        typedef const BOOL(__cdecl* RENDERERSETSTENCILFUNC) (const Renderer::Graphics::StencilFunction);
        typedef const BOOL(__cdecl* RENDERERSETSTENCILPASS) (const Renderer::Graphics::StencilPass);
        typedef const BOOL(__cdecl* RENDERERSETTEXTURECLAMP) (const Renderer::Graphics::TextureClamp, const Renderer::Graphics::TextureClamp, const u32);
        typedef const BOOL(__cdecl* RENDERERSETTRANSFORM) (const struct Renderer::Graphics::Transform*, const f32, const f32);
        typedef const BOOL(__cdecl* RENDERERSETTRANSFORMO2W) (const struct Mathematics::Matrix4x4*);
        typedef const BOOL(__cdecl* RENDERERSETTRANSFORMW2L) (const struct Mathematics::Matrix4x4*, const f32, const f32);
        typedef const BOOL(__cdecl* RENDERERSETVIDEOMODE) (const u32, const u32, const u32, u32*, const u32);
        typedef const BOOL(__cdecl* RENDERERSETVIEWPORT) (const f32, const f32, const f32, const f32, const f32, const f32);
        typedef const BOOL(__cdecl* RENDERERTOGGLE) (void);
        typedef const BOOL(__cdecl* RENDERERUNLOCKFRAME) (void);
        typedef const BOOL(__cdecl* RENDERERVIDEORESTORE) (void);
        typedef const BOOL(__cdecl* RENDERERVIDEOSAVE) (void);
        typedef const u32(__cdecl* RENDERERALLOCATETEXTURE) (const struct Renderer::Graphics::Texture*);
        typedef const u32(__cdecl* RENDERERCREATEGAMEWINDOW) (const u32, const u32, const u32, const u32*, const HWND);
        typedef const u32(__cdecl* RENDERERSETGAMEWINDOWMODE) (const BOOL);
        typedef const u32(__cdecl* RENDERERUPLOADTEXTURE) (const u32, const struct Renderer::Graphics::Texture*);
    }
}