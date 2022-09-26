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
#include "Mathematics/Vector.h"
#include "Win.h"

namespace Renderer
{
    namespace External
    {
        extern "C" BOOL __cdecl AllocateOptimizedMesh(void);
        extern "C" u32 __cdecl AllocateTexture(const struct Renderer::Graphics::Texture*);
        extern "C" BOOL __cdecl AllocateVertexBuffer(const void*, const u32, const u32, void**);
        extern "C" BOOL __cdecl BeginCubeMapRender(const u32, const u32);
        extern "C" BOOL __cdecl BeginRenderToTexture(const u32);
        extern "C" BOOL __cdecl BeginScene(void);
        extern "C" BOOL __cdecl Clear(const u32, const u32);
        extern "C" BOOL __cdecl ClearZBuffer(void);
        extern "C" BOOL __cdecl CopyBackBufferToRenderTexture(void);
        extern "C" u32 __cdecl CreateGameWindow(const u32, const u32, const u32, u32*, const HWND);
        extern "C" BOOL __cdecl CreateGraphicsCardList(u32*, char**, char**, u32*, u32*);
        extern "C" BOOL __cdecl DestroyGameWindow(const u32);
        extern "C" BOOL __cdecl DotListD3DL(const struct Renderer::Graphics::D3DLVertex*, const u32, const u32);
        extern "C" BOOL __cdecl DotListD3DTL(const struct Renderer::Graphics::D3DTLVertex*, const u32, const u32);
        extern "C" BOOL __cdecl DrawPixels(void);
        extern "C" BOOL __cdecl EnableClipping(const Renderer::Graphics::ClippingMode);
        extern "C" BOOL __cdecl EnableCulling(const Renderer::Graphics::CullMode);
        extern "C" BOOL __cdecl EnableLighting(const BOOL);
        extern "C" BOOL __cdecl EndCubeMapRender(void);
        extern "C" BOOL __cdecl EndRenderToTexture(void);
        extern "C" BOOL __cdecl EndScene(void);
        extern "C" BOOL __cdecl FreeOptimizedMesh(void);
        extern "C" BOOL __cdecl FreeTexture(const u32);
        extern "C" BOOL __cdecl FreeVertexBuffer(const void*);
        extern "C" BOOL __cdecl GetTextureInfo(const u32);
        extern "C" BOOL __cdecl HasShaderSupport(void);
        extern "C" BOOL __cdecl Init(const HWND, const struct InitializeModuleArguments*);
        extern "C" BOOL __cdecl Kill(void);
        extern "C" BOOL __cdecl LineListD3DL(const struct Renderer::Graphics::D3DLVertex*, const u32, const u32);
        extern "C" BOOL __cdecl LineListD3DTL(const struct Renderer::Graphics::D3DTLVertex*, const u32, const u32);
        extern "C" BOOL __cdecl LockFrame(void);

        /// <summary>
        /// Draws level geometry, characters, and water.
        /// </summary>
        extern "C" BOOL __cdecl PolyList(const struct Renderer::Graphics::SVertex*, const u32, const u16*, const u32, const u32);
        extern "C" BOOL __cdecl PolyListBasis(const struct Renderer::Graphics::SVertexBasis*, const u32, const u16*, const u32, const u32);
        extern "C" BOOL __cdecl PolyListBone(const void*, const u32, const u16*, const u32, const u32, const u32);

        /// <summary>
        /// Draws sky, lights, active moving fog.
        /// </summary>
        extern "C" BOOL __cdecl PolyListD3DL(const struct Renderer::Graphics::D3DTLVertex*, const u32, const u16*, const u32, const u32);

        /// <summary>
        /// Draws UI elements such as main menu, loading screen, and HUD during the play.
        /// </summary>
        extern "C" BOOL __cdecl PolyListD3DTL(const struct Renderer::Graphics::D3DTLVertex*, const u32, const u16*, const u32, const u32);

        /// <summary>
        /// Draws sky box and special effects, both environment effects and character effects.
        /// The method is also used as a fallback in case PolyList is not exported.
        /// </summary>
        extern "C" BOOL __cdecl PolyListTL(const struct Renderer::Graphics::TLVertex*, const u32, const u16*, const u32, const u32);
        extern "C" BOOL __cdecl RenderOptimizedMesh(void);
        extern "C" BOOL __cdecl RenderVertexBuffer(const void*, const u32, const u16*, const u32, const u32);
        extern "C" BOOL __cdecl RenderVertexBufferBasis(const void*, const u32, const u16*, const u32, const u32);
        extern "C" BOOL __cdecl RenderVertexBufferBone(const void*, const u32, const u16*, const u32, const struct Mathematics::Vector4*, const u32, const u32);
        extern "C" BOOL __cdecl RenderVertexBufferPrelit(const void*, const u32, const u16*, const u32, const u32);
        extern "C" BOOL __cdecl ResetTextureCache(void);
        extern "C" BOOL __cdecl RestoreVideoMode(void);
        extern "C" BOOL __cdecl SelectCubeTexture(const u32, const u32);
        extern "C" BOOL __cdecl SelectGameWindow(const s32, const s32);
        extern "C" BOOL __cdecl SelectGraphicsCard(const u32);
        extern "C" BOOL __cdecl SelectRenderTexture(const u32);
        extern "C" BOOL __cdecl SelectTexture(const u32, const u32);
        extern "C" BOOL __cdecl SetAlpha(const u32);
        extern "C" BOOL __cdecl SetAmbientLight(const f32);
        extern "C" BOOL __cdecl SetAmbientLightColor(const f32, const f32, const f32);
        extern "C" BOOL __cdecl SetColorTable(const u8*, u16*);
        extern "C" BOOL __cdecl SetDestinationBlend(const Renderer::Graphics::TextureDestinationBlend);
        extern "C" BOOL __cdecl SetFog(const f32, const f32, const f32, const f32, const f32);
        extern "C" BOOL __cdecl SetGloss(const f32);
        extern "C" BOOL __cdecl SetLightColor(const f32, const f32, const f32, const u32);
        extern "C" BOOL __cdecl SetLightConstants(const f32, const f32, const f32, const f32);
        extern "C" BOOL __cdecl SetLightVector(const Mathematics::Vector3*, const f32, const u32);
        extern "C" BOOL __cdecl SetMultiTextureBlend(const Renderer::Graphics::TextureBlendOperation);
        extern "C" BOOL __cdecl SetOmniLight(const f32*, const f32, const u32);
        extern "C" BOOL __cdecl SetSourceBlend(const Renderer::Graphics::TextureSourceBlend);
        extern "C" BOOL __cdecl SetStencilEnable(const BOOL);
        extern "C" BOOL __cdecl SetStencilFunc(const Renderer::Graphics::StencilFunction);
        extern "C" BOOL __cdecl SetStencilPass(const Renderer::Graphics::StencilPass);
        extern "C" BOOL __cdecl SetTextureClamp(const Renderer::Graphics::TextureClamp, const Renderer::Graphics::TextureClamp, const u32);
        extern "C" BOOL __cdecl SetTransform(const struct Renderer::Graphics::Transform*, const f32, const f32);
        extern "C" BOOL __cdecl SetTransformO2W(const struct Mathematics::Matrix4x4*);
        extern "C" BOOL __cdecl SetTransformW2L(const struct Mathematics::Matrix4x4*, const f32, const f32);
        extern "C" BOOL __cdecl SetVideoMode(const u32, const u32, const u32, u32*, const u32);
        extern "C" BOOL __cdecl SetViewPort(const f32, const f32, const f32, const f32, const f32, const f32);
        extern "C" BOOL __cdecl SetGameWindowMode(const BOOL);
        extern "C" BOOL __cdecl Toggle(void);
        extern "C" BOOL __cdecl UnlockFrame(void);
        extern "C" BOOL __cdecl UploadTexture(const u32, const struct Renderer::Graphics::Texture*);
        extern "C" BOOL __cdecl VideoRestore(void);
        extern "C" BOOL __cdecl VideoSave(void);
        extern "C" BOOL __cdecl IsGameWindowMode(void);
        extern "C" BOOL __cdecl LockFrameForScreenshot(void);
        extern "C" BOOL __cdecl SetDirtyRect(const u32, const u32, const u32, const u32);
    }
}