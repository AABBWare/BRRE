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
#include "Settings.h"

#define MIN_SUPPORTED_SHADER_VERSION 0x100
#define SHADER_VERSION_MASK 0xff00

extern "C" u32 __cdecl CreateGameWindow(const s32 width, const s32 height, const s32 bpp, const s32 unk4, const HWND hwnd)
{
    if (!State.Window.IsWindowMode) { return 0; }

    for (auto x = 1; x < MAX_WINDOW_COUNT; x++)
    {
        if (State.DX.Windows.Windows[x] != NULL) { continue; }

        auto dxw = new DXWindow();

        DXWindowConstructor(dxw);

        dxw->Width = width;
        dxw->Height = height;
        dxw->BitsPerPixel = bpp;
        dxw->Unknown4 = unk4;

        // TODO: memcpy unk4?
        dxw->HWND = hwnd;

        State.DX.Windows.Windows[x] = dxw;

        DXWindowInitialize(dxw, &State.DX.CurrentDevice.Capabilities.SwapChainPresentParameters);

        DXC(State.DX.DirectXDevice->CreateAdditionalSwapChain(&State.DX.CurrentDevice.Capabilities.SwapChainPresentParameters,
            &dxw->SwapChain), "Unable to create additional swap chain.");

        DXC(State.DX.DirectXDevice->CreateDepthStencilSurface(width, height,
            State.DX.CurrentDevice.Capabilities.SwapChainPresentParameters.AutoDepthStencilFormat, D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_NONE,
            &dxw->DepthSurface), "Unable to create additional depth buffer.");

        return x;
    }

    return 0;
}

extern "C" u32 __cdecl DestroyGameWindow(const s32 indx)
{
    auto dxw = State.DX.Windows.Windows[indx];

    if (dxw == NULL) { return FALSE; }

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

    delete dxw;

    State.DX.Windows.Windows[indx] = NULL;

    return TRUE;
}

extern "C" u32 __cdecl SelectGameWindow(const s32 indx, const s32 unk4)
{
    if (indx < 0 || MAX_WINDOW_COUNT <= indx) { return FALSE; }

    auto dxw = State.DX.Windows.Windows[indx];
    auto cdxw = State.DX.Windows.Current;

    if (dxw != NULL)
    {
        // todo: memcopy Unk4?

        if (indx == 0)
        {
            DXC(State.DX.DirectXDevice->GetBackBuffer(0,
                D3DBACKBUFFER_TYPE::D3DBACKBUFFER_TYPE_MONO, &State.DX.Surfaces.DrawSurface),
                "Unable to get back buffer.");

            DXC(State.DX.DirectXDevice->SetRenderTarget(State.DX.Surfaces.DrawSurface, State.DX.Surfaces.DepthSurface),
                "Unable to set primary render target to primary window.");
        }
        else
        {
            if (dxw->SwapChain == NULL)
            {
                Exit("Unable to select window %d.", indx);
                State.DX.Windows.Current = dxw;

                return FALSE;
            }

            DXC(dxw->SwapChain->GetBackBuffer(0,
                D3DBACKBUFFER_TYPE::D3DBACKBUFFER_TYPE_MONO, &State.DX.Surfaces.DrawSurface),
                "Unable to get back buffer from additional swap chain.");

            DXC(State.DX.DirectXDevice->SetRenderTarget(State.DX.Surfaces.DrawSurface, dxw->DepthSurface),
                "Unable to set render target to secondary window.");
        }

        cdxw = dxw;

        if (State.DX.Surfaces.DrawSurface != NULL)
        {
            State.DX.Surfaces.DrawSurface->Release();
            State.DX.Surfaces.DrawSurface = NULL;

            State.DX.Windows.Current = dxw;

            return TRUE;
        }
    }

    State.DX.Windows.Current = cdxw;

    return TRUE;
}

extern "C" u32 __cdecl SetGameWindowMode(const s32 mode)
{
    State.Window.IsWindowMode = mode;

    return TRUE;
}

extern "C" u32 __cdecl RestoreVideoMode(void)
{
    DXDestroy();

    if (State.DX.DirectXDevice != NULL)
    {
        State.DX.DirectXDevice->Release();
        State.DX.DirectXDevice = NULL;
    }

    if (State.DX.DirectX != NULL)
    {
        State.DX.DirectX->Release();
        State.DX.DirectX = NULL;
    }

    if (!State.Window.IsWindowMode)
    {
        ShowCursor(TRUE);
    }

    return TRUE;
}

extern "C" u32 __cdecl SetVideoMode(const s32 width, const s32 height, const s32 bpp, const s32 unk4, const s32 indx)
{
    auto dxw = State.DX.Windows.Windows[indx];

    if (dxw == NULL)
    {
        Exit("Invalid window %d.", indx);
        return FALSE;
    }

    if (State.DX.DirectX != NULL)
    {
        if (indx == 0)
        {
            return DXResetDevice(width, height, bpp, unk4, dxw);
        }

        return DXWindowInitialize(width, height, indx);
    }

    dxw->Width = width;
    dxw->Height = height;
    dxw->BitsPerPixel = bpp;

    State.DX.Mode.Mode = 0;

    SettingsReadValue("Graphics", "useFixedFunctionPipe", (u32*)&State.Settings.UseFixedFunctionPipe);
    SettingsReadValue("Graphics", "syncRetrace", (u32*)&State.Settings.SyncRetrace);
    SettingsReadValue("Graphics", "renderTextureCount", &State.Settings.RenderTextureCount);
    SettingsReadValue("Graphics", "renderTextureSize", &State.Settings.RenderTextureSize);
    SettingsReadValue("Graphics", "cubeMapCount", &State.Settings.CubeTextureCount);
    SettingsReadValue("Graphics", "cubeMapSize", &State.Settings.CubeTextureSize);
    SettingsReadValue("Graphics", "useWBuffer", (u32*)&State.Settings.UseWBuffer);

    if (State.Settings.UseWBuffer != 0)
    {
        State.DX.ZBufferType = D3DZBUFFERTYPE::D3DZB_USEW;
    }

    // todo: handle unk4 (memcopy)

    DXDestroy();

    State.DX.DirectX = Direct3DCreate8(D3D_SDK_VERSION);

    if (State.DX.DirectX == NULL)
    {
        Exit("Unable to initialize DirectX.");
        return FALSE;
    }

    State.DX.DirectX->GetDeviceCaps(State.DX.CurrentDevice.Index, D3DDEVTYPE::D3DDEVTYPE_HAL,
        &State.DX.CurrentDevice.Capabilities.DisplayCapabilities);

    DXWindowInitialize(dxw, &State.DX.CurrentDevice.Capabilities.DevicePresentParameters);

    auto vertexShaderVersion = State.DX.CurrentDevice.Capabilities.DisplayCapabilities.VertexShaderVersion & SHADER_VERSION_MASK;
    auto pixelShaderVersion = State.DX.CurrentDevice.Capabilities.DisplayCapabilities.PixelShaderVersion & SHADER_VERSION_MASK;

    if (vertexShaderVersion < MIN_SUPPORTED_SHADER_VERSION || pixelShaderVersion < MIN_SUPPORTED_SHADER_VERSION)
    {
        State.Settings.UseFixedFunctionPipe = 1;

        State.DX.CurrentDevice.Capabilities.DisplayCapabilities.VertexShaderVersion = 0;
        State.DX.CurrentDevice.Capabilities.DisplayCapabilities.PixelShaderVersion = 0;
    }

    auto result = State.DX.DirectX->CreateDevice(State.DX.CurrentDevice.Index, D3DDEVTYPE::D3DDEVTYPE_HAL,
        State.Window.HWND, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE,
        &State.DX.CurrentDevice.Capabilities.DevicePresentParameters, &State.DX.DirectXDevice);

    if (result != 0)
    {
        State.DX.CurrentDevice.Capabilities.DevicePresentParameters.BackBufferCount = 1;

        result = State.DX.DirectX->CreateDevice(State.DX.CurrentDevice.Index, D3DDEVTYPE::D3DDEVTYPE_HAL,
            State.Window.HWND, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE,
            &State.DX.CurrentDevice.Capabilities.DevicePresentParameters, &State.DX.DirectXDevice);

        if (result != 0)
        {
            Exit("Unable to create device.");
            return FALSE;
        }
    }

    DXCreate();

    if (!State.Window.IsWindowMode)
    {
        SetCursorPos(dxw->Width - 1, dxw->Height - 1);
        ShowCursor(FALSE);
    }

    Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0);
    Toggle();

    return TRUE;
}

extern "C" u32 __cdecl VideoRestore(void)
{
    DXC(State.DX.DirectXDevice->Reset(&State.DX.CurrentDevice.Capabilities.DevicePresentParameters),
        "Unable to reset device.");

    DXCreate();

    return TRUE;
}

extern "C" u32 __cdecl VideoSave(void)
{
    DXDestroy();

    return TRUE;
}