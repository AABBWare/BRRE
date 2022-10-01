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
#include "Mathematics.h"
#include "Module.h"
#include "State.h"

using namespace Mathematics;
using namespace Renderer::Enums;

namespace Renderer::Module
{
    extern "C" BOOL __cdecl CreateGraphicsCardList(u32 * count, char** apis, char** names, u32 * ssids, u32 * dids)
    {
        *count = State.DX.AvailableDevices.Count;

        for (auto x = 0; x < State.DX.AvailableDevices.Count; x++)
        {
            apis[x] = const_cast<char*>("DirectX 8.1");
            names[x] = State.DX.AvailableDevices.Details[x].Description;

            dids[x] = State.DX.AvailableDevices.Details[x].DeviceID;
            ssids[x] = State.DX.AvailableDevices.Details[x].SubSystemID;
        }

        return TRUE;
    }

    extern "C" BOOL __cdecl DrawPixels(void)
    {
        return FALSE;
    }

    extern "C" BOOL __cdecl SelectGraphicsCard(const u32 indx)
    {
        State.DX.CurrentDevice.Index = indx;

        return Init(State.Window.HWND, &State.InitializeArguments);
    }

    extern "C" BOOL __cdecl SetFog(const f32 r, const f32 g, const f32 b, const f32 start, const f32 end)
    {
        auto rv = Clamp((s32)(r * 255.0f), 0, 255);
        auto gv = Clamp((s32)(g * 255.0f), 0, 255);
        auto bv = Clamp((s32)(b * 255.0f), 0, 255);

        auto color = (rv << 16) | (gv << 8) | bv;

        DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGCOLOR, color);
        DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGSTART, (u32)start);
        DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGEND, (u32)end);
        DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_FOGDENSITY, 0);

        State.DX.Light.IsChanged = TRUE;

        State.DX.FogStart = start;
        State.DX.FogEnd = end;

        State.DX.FogR = r;
        State.DX.FogG = g;
        State.DX.FogB = b;

        return TRUE;
    }

    extern "C" BOOL __cdecl EnableClipping(const ClippingMode)
    {
        return TRUE;
    }

    extern "C" BOOL __cdecl EnableCulling(const CullMode mode)
    {
        switch (mode)
        {
        case CullMode::CCW:
        {
            State.DX.Mode.Cull = D3DCULL::D3DCULL_CCW;
            break;
        }
        case CullMode::CW:
        {
            State.DX.Mode.Cull = D3DCULL::D3DCULL_CW;
            break;
        }
        default:
        {
            State.DX.Mode.Cull = D3DCULL::D3DCULL_NONE;
            break;
        }
        }

        return TRUE;
    }

    extern "C" BOOL __cdecl Clear(const u32 options, const u32 color)
    {
        auto value = options & D3DCLEAR_TARGET;

        if (options & D3DCLEAR_ZBUFFER)
        {
            value = value | D3DCLEAR_ZBUFFER;
        }

        DX::DXC(State.DX.DirectXDevice->Clear(0, NULL, value, color, 1.0f, 0), "Unable to clear.");

        return TRUE;
    }

    extern "C" BOOL __cdecl Toggle(void)
    {
        if (State.DX.DirectXDevice != NULL)
        {
            if (State.Window.IsWindowMode)
            {
                State.DX.DirectXDevice->Present(NULL, NULL, NULL, NULL);

                for (auto x = 1; x < MAX_WINDOW_COUNT; x++)
                {
                    auto window = State.DX.Windows.Windows[x];

                    if (window == NULL) { continue; }

                    auto chain = window->SwapChain;

                    if (chain != NULL)
                    {
                        chain->Present(NULL, NULL, NULL, NULL);
                    }
                }

                return TRUE;
            }

            DX::DXC(State.DX.DirectXDevice->Present(NULL, NULL, NULL, NULL), "Unable to present back buffer.");
        }

        return TRUE;
    }

    extern "C" BOOL __cdecl SetAlpha(const u32 value)
    {
        DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, value);

        return TRUE;
    }

    extern "C" BOOL __cdecl SetGloss(const f32)
    {
        return TRUE;
    }

    extern "C" BOOL __cdecl SetColorTable(const u8 * rgb, u16 * rgba)
    {
        auto rs = State.DX.Windows.Current->BitsPerPixel == BITS_PER_PIXEL_16 ? 8 : 1;
        auto gs = State.DX.Windows.Current->BitsPerPixel == BITS_PER_PIXEL_16 ? 8 : 1;
        auto bs = State.DX.Windows.Current->BitsPerPixel == BITS_PER_PIXEL_16 ? 4 : 1;

        if (State.DX.Windows.Current->BitsPerPixel == BITS_PER_PIXEL_16)
        {
            *State.InitializeArguments.R.Display = 11;
            *State.InitializeArguments.G.Display = 5;
        }
        else
        {
            *State.InitializeArguments.R.Display = 16;
            *State.InitializeArguments.G.Display = 8;
        }

        auto rsh = *State.InitializeArguments.R.Display;
        auto gsh = *State.InitializeArguments.G.Display;

        for (auto x = 0; x < 256; x++)
        {
            auto r = rgb[x * 3 + 0] / rs << (rsh & 0x1f);
            auto g = rgb[x * 3 + 1] / bs << (gsh & 0x1f);
            auto b = rgb[x * 3 + 2] / gs << (0 & 0x1f);

            rgba[x] = (u16)(r | g | b);
        }

        return TRUE;
    }

    extern "C" BOOL __cdecl HasShaderSupport(void)
    {
        return !State.Settings.IsFixedPipelineActive;
    }
}