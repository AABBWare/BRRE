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
#include "DXWindow.h"
#include "DXShader.h"
#include "Graphics/Matrix.h"
#include "Win.h"

#include <d3dx8.h>

void DXC(const HRESULT, const char*);

u32 DXAlpha(const f32);
u32 DXColor(const f32, const f32, const f32, const f32);
u32 DXInitializeState(void);
u32 DXResetDevice(const s32, const s32, const s32, const s32, struct DXWindow*);
u32 DXSetLight(void);
void DXCreate(void);
void DXDestroy(void);
void DXSetBlendShaders(const u32);
void DXSetIndexes(const u16*, const u32);
void DXSetIndexSource(const u32, IDirect3DIndexBuffer8*);
void DXSetLightShaders(const u32);
void DXSetMode(const u32);
void DXSetRenderState(const D3DRENDERSTATETYPE, const s32);
void DXSetShaders(const VertexShaderName, const PixelShaderName);
void DXSetShadersMode(const u32);
void DXSetSkeletonShaders(const u32);
void DXSetTextureStageValue(const u32, const D3DTEXTURESTAGESTATETYPE, const u32);
void DXSetTLShaders(const u32);
void DXSetVertexShaderValues(const u32);