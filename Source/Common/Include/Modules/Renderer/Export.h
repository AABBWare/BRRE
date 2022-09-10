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

extern "C" u32 __cdecl AllocateOptimizedMesh(void);
extern "C" u32 __cdecl AllocateTexture(const struct Texture*);
extern "C" u32 __cdecl AllocateVertexBuffer(const void*, const u32, const u32, void**);
extern "C" u32 __cdecl BeginCubeMapRender(const u32, const u32);
extern "C" u32 __cdecl BeginRenderToTexture(const u32);
extern "C" u32 __cdecl BeginScene(void);
extern "C" u32 __cdecl Clear(const u32, const u32);
extern "C" u32 __cdecl ClearZBuffer(void);
extern "C" u32 __cdecl CopyBackBufferToRenderTexture(void);
extern "C" u32 __cdecl CreateGameWindow(const s32, const s32, const s32, const s32, const HWND);
extern "C" u32 __cdecl CreateGraphicsCardList(s32*, char**, char**, u32*, u32*);
extern "C" u32 __cdecl DestroyGameWindow(const s32);
extern "C" u32 __cdecl DotListD3DTL(const struct D3DTLVertex*, const u32, const u32);
extern "C" u32 __cdecl DrawPixels(void);
extern "C" u32 __cdecl EnableClipping(const u32);
extern "C" u32 __cdecl EnableCulling(const CullMode);
extern "C" u32 __cdecl EnableLighting(const u32);
extern "C" u32 __cdecl EndCubeMapRender(void);
extern "C" u32 __cdecl EndRenderToTexture(void);
extern "C" u32 __cdecl EndScene(void);
extern "C" u32 __cdecl FreeOptimizedMesh(void);
extern "C" u32 __cdecl FreeTexture(const u32);
extern "C" u32 __cdecl FreeVertexBuffer(const void*);
extern "C" u32 __cdecl GetTextureInfo(const u32);
extern "C" u32 __cdecl HasShaderSupport(void);
extern "C" u32 __cdecl Init(const HWND, const struct InitializeModuleArguments*);
extern "C" u32 __cdecl Kill(void);
extern "C" u32 __cdecl LineListD3DTL(const struct D3DTLVertex*, const u32, const u32);
extern "C" u32 __cdecl LockFrame(void);

/// <summary>
/// Draws level geometry, characters, and water.
/// </summary>
extern "C" u32 __cdecl PolyList(const struct SVertex*, const u32, const u16*, const u32, const u32);
extern "C" u32 __cdecl PolyListBasis(const struct SVertexBasis*, const u32, const u16*, const u32, const u32);
extern "C" u32 __cdecl PolyListBone(const void*, const u32, const u16*, const u32, const u32, const u32);

/// <summary>
/// Draws sky, lights, active moving fog.
/// </summary>
extern "C" u32 __cdecl PolyListD3DL(const struct D3DTLVertex*, const u32, const u16*, const u32, const u32);

/// <summary>
/// Draws UI elements such as main menu, loading screen, and HUD during the gameplay.
/// </summary>
extern "C" u32 __cdecl PolyListD3DTL(const struct D3DTLVertex*, const u32, const u16*, const u32, const u32);

/// <summary>
/// Draws sky box and special effects, both environment effects and character effects.
/// The method is also used as a fallback in case PolyList is not exported.
/// </summary>
extern "C" u32 __cdecl PolyListTL(const struct TLVertex*, const u32, const u16*, const u32, const u32);
extern "C" u32 __cdecl RenderOptimizedMesh(void);
extern "C" u32 __cdecl RenderVertexBuffer(const void*, const u32, const u16*, const u32, const u32);
extern "C" u32 __cdecl RenderVertexBufferBasis(const void*, const u32, const u16*, const u32, const u32);
extern "C" u32 __cdecl RenderVertexBufferBone(const void*, const u32, const u16*, const u32, const struct Vector4*, const u32, const u32);
extern "C" u32 __cdecl RenderVertexBufferPrelit(const void*, const u32, const u16*, const u32, const u32);
extern "C" u32 __cdecl ResetTextureCache(void);
extern "C" u32 __cdecl RestoreVideoMode(void);
extern "C" u32 __cdecl SelectCubeTexture(const u32, const u32);
extern "C" u32 __cdecl SelectGameWindow(const s32, const s32);
extern "C" u32 __cdecl SelectGraphicsCard(const s32);
extern "C" u32 __cdecl SelectRenderTexture(const u32);
extern "C" u32 __cdecl SelectTexture(const u32, const u32);
extern "C" u32 __cdecl SetAlpha(const u32);
extern "C" u32 __cdecl SetAmbientLight(const f32);
extern "C" u32 __cdecl SetAmbientLightColor(const f32, const f32, const f32);
extern "C" u32 __cdecl SetColorTable(const u8*, u16*);
extern "C" u32 __cdecl SetDestBlend(const TextureDestinationBlend);
extern "C" u32 __cdecl SetFog(const f32, const f32, const f32, const f32, const f32);
extern "C" u32 __cdecl SetGloss(const f32);
extern "C" u32 __cdecl SetLightColor(const f32, const f32, const f32, const s32);
extern "C" u32 __cdecl SetLightConstants(const f32, const f32, const f32, const f32);
extern "C" u32 __cdecl SetLightVector(void);
extern "C" u32 __cdecl SetMultiTextureBlend(const TextureBlendOperation);
extern "C" u32 __cdecl SetOmniLight(const f32*, const f32, const u32);
extern "C" u32 __cdecl SetSrcBlend(const TextureSourceBlend);
extern "C" u32 __cdecl SetStencilEnable(const u32);
extern "C" u32 __cdecl SetStencilFunc(const StencilFunction);
extern "C" u32 __cdecl SetStencilPass(const StencilPass);
extern "C" u32 __cdecl SetTextureClamp(const TextureClamp, const TextureClamp, const u32);
extern "C" u32 __cdecl SetTransform(const struct Transform*, const f32, const f32);
extern "C" u32 __cdecl SetTransformO2W(const struct Matrix4x4*);
extern "C" u32 __cdecl SetTransformW2L(const struct Matrix4x4*, const f32, const f32);
extern "C" u32 __cdecl SetVideoMode(const s32, const s32, const s32, const s32, const s32);
extern "C" u32 __cdecl SetViewport(const f32, const f32, const f32, const f32, const f32, const f32);
extern "C" u32 __cdecl SetGameWindowMode(const s32);
extern "C" u32 __cdecl Toggle(void);
extern "C" u32 __cdecl UnlockFrame(void);
extern "C" u32 __cdecl UploadTexture(const u32, const struct Texture*);
extern "C" u32 __cdecl VideoRestore(void);
extern "C" u32 __cdecl VideoSave(void);