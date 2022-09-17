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
#include "State.h"

namespace Renderer
{
    namespace External
    {
        namespace DX
        {
            namespace Window
            {
                void Initialize(struct DXWindow* window)
                {
                    window->Width = 640;
                    window->Height = 480;
                    window->BitsPerPixel = BITS_PER_PIXEL_32;
                    window->Unknown4 = NULL;
                    window->SwapChain = NULL;
                    window->DepthSurface = NULL;
                }

                void Initialize(struct DXWindow* window, D3DPRESENT_PARAMETERS* pp)
                {
                    D3DDISPLAYMODE displayMode;

                    if (!State.Window.IsWindowMode)
                    {
                        s32 selected = -1;
                        auto count = (s32)State.DX.DirectX->GetAdapterModeCount(State.DX.CurrentDevice.Index);

                        for (auto x = 0; x < count; x++)
                        {
                            DXC(State.DX.DirectX->EnumAdapterModes(State.DX.CurrentDevice.Index, x, &displayMode),
                                "Unable to enumerate device modes.");

                            if (displayMode.Width == window->Width && displayMode.Height == window->Height)
                            {
                                if (window->BitsPerPixel == BITS_PER_PIXEL_16)
                                {
                                    if (displayMode.Format == D3DFORMAT::D3DFMT_R5G6B5)
                                    {
                                        selected = x;
                                        break;
                                    }
                                }
                                else if (displayMode.Format == D3DFORMAT::D3DFMT_R8G8B8 || displayMode.Format == D3DFORMAT::D3DFMT_A8R8G8B8 || displayMode.Format == D3DFORMAT::D3DFMT_X8R8G8B8)
                                {
                                    selected = x;
                                    break;
                                }
                            }
                        }

                        if (selected == -1)
                        {
                            Exit("Unable to find a valid video mode for %d x %d.", window->Width, window->Height);
                            return;
                        }
                    }
                    else
                    {
                        DXC(State.DX.DirectX->GetAdapterDisplayMode(State.DX.CurrentDevice.Index, &displayMode),
                            "Unable to get display mode for window mode.");
                    }

                    auto stencilFormat = window->BitsPerPixel == BITS_PER_PIXEL_16 ? D3DFORMAT::D3DFMT_D16 : D3DFORMAT::D3DFMT_D32;

                    if (window->BitsPerPixel != BITS_PER_PIXEL_16)
                    {
                        auto result = State.DX.DirectX->CheckDeviceFormat(State.DX.CurrentDevice.Index, D3DDEVTYPE::D3DDEVTYPE_HAL,
                            displayMode.Format, D3DUSAGE_DEPTHSTENCIL, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_D32);

                        if (result != 0)
                        {
                            stencilFormat = D3DFORMAT::D3DFMT_D24S8;

                            result = State.DX.DirectX->CheckDeviceFormat(State.DX.CurrentDevice.Index, D3DDEVTYPE::D3DDEVTYPE_HAL,
                                displayMode.Format, D3DUSAGE_DEPTHSTENCIL, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_D24S8);

                            if (result != 0)
                            {
                                stencilFormat = D3DFORMAT::D3DFMT_D16;

                                DXC(State.DX.DirectX->CheckDeviceFormat(State.DX.CurrentDevice.Index, D3DDEVTYPE::D3DDEVTYPE_HAL,
                                    displayMode.Format, D3DUSAGE_DEPTHSTENCIL, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_D16),
                                    "Unable to find depth buffer format.");

                                return;
                            }
                        }
                    }

                    pp->BackBufferWidth = window->Width;
                    pp->BackBufferHeight = window->Height;
                    pp->BackBufferFormat = displayMode.Format;
                    pp->AutoDepthStencilFormat = stencilFormat;
                    pp->Windowed = State.Window.IsWindowMode;
                    pp->MultiSampleType = D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_NONE;
                    pp->EnableAutoDepthStencil = TRUE;
                    pp->Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
                    pp->hDeviceWindow = window->HWND;

                    if (!State.Window.IsWindowMode)
                    {
                        pp->BackBufferCount = 2;
                        pp->SwapEffect = D3DSWAPEFFECT::D3DSWAPEFFECT_FLIP;
                        pp->FullScreen_RefreshRateInHz = displayMode.RefreshRate;
                        pp->FullScreen_PresentationInterval = State.Settings.SyncRetrace == 0 ? D3DPRESENT_INTERVAL_IMMEDIATE : D3DPRESENT_INTERVAL_TWO;

                        return;
                    }

                    pp->BackBufferCount = 1;
                    pp->SwapEffect = D3DSWAPEFFECT::D3DSWAPEFFECT_COPY;
                    pp->FullScreen_RefreshRateInHz = 0;
                    pp->FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
                }

                BOOL Initialize(const u32 width, const u32 height, const u32 indx)
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

                    dxw->Width = width;
                    dxw->Height = height;

                    Initialize(dxw, &State.DX.CurrentDevice.Capabilities.SwapChainPresentParameters);

                    DXC(State.DX.DirectXDevice->CreateAdditionalSwapChain(&State.DX.CurrentDevice.Capabilities.SwapChainPresentParameters,
                        &dxw->SwapChain), "Unable to create additional swap chain.");

                    DXC(State.DX.DirectXDevice->CreateDepthStencilSurface(dxw->Width, dxw->Height,
                        State.DX.CurrentDevice.Capabilities.SwapChainPresentParameters.AutoDepthStencilFormat,
                        D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_NONE, &dxw->DepthSurface),
                        "Unable to create additional depth buffer.");

                    return TRUE;
                }
            }
        }
    }
}