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
#include "DX.Shader.h"
#include "DX.Window.h"
#include "Mathematics.Matrix.h"
#include "Win.h"

#include <d3dx8.h>

namespace DX
{
    void DXC(const HRESULT, const char*);

    BOOL InitializeState(void);
    BOOL ResetDevice(const u32, const u32, const u32, u32*, struct DX::DXWindow*);
    BOOL SetLight(void);
    u32 Alpha(const f32);
    u32 Color(const f32, const f32, const f32, const f32);
    void Create(void);
    void Destroy(void);
    void SetBlendShaders(const u32);
    void SetIndexes(const u16*, const u32);
    void SetIndexSource(const u32, IDirect3DIndexBuffer8*);
    void SetLightShaders(const u32);
    void SetMode(const u32);
    void SetRenderState(const D3DRENDERSTATETYPE, const s32);
    void SetShaders(const DX::Shaders::DXVertexShaderName, const DX::Shaders::DXPixelShaderName);
    void SetShadersMode(const u32);
    void SetSkeletonShaders(const u32);
    void SetTextureStageValue(const u32, const D3DTEXTURESTAGESTATETYPE, const u32);
    void SetTLShaders(const u32);
    void SetVertexShaderValues(const u32);
}