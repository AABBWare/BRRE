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
#include "Exit.h"
#include "Module.h"
#include "State.h"

u32 ChangeWindowMode()
{
    DEVMODEA dm = { 0 };

    dm.dmSize = sizeof(dm);
    dm.dmPelsWidth = State.Window.Width;
    dm.dmPelsHeight = State.Window.Height;
    dm.dmBitsPerPel = State.Window.BitsPerPixel;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

    if (!State.Window.IsWindowMode)
    {
        if (ChangeDisplaySettingsA(&dm, CDS_FULLSCREEN))
        {
            Exit("Unable to change display settings.");
            return FALSE;
        }
    }

    GLRelease();

    PIXELFORMATDESCRIPTOR pfd =
    {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = BITS_PER_PIXEL_32,
        .cRedBits = 0,
        .cRedShift = 0,
        .cGreenBits = 0,
        .cGreenShift = 0,
        .cBlueBits = 0,
        .cBlueShift = 0,
        .cAlphaBits = 0,
        .cAlphaShift = 0,
        .cAccumBits = 0,
        .cAccumRedBits = 0,
        .cAccumGreenBits = 0,
        .cAccumBlueBits = 0,
        .cAccumAlphaBits = 0,
        .cDepthBits = BITS_PER_PIXEL_32,
        .cStencilBits = 0,
        .cAuxBuffers = 0,
        .iLayerType = 0,
        .bReserved = 0,
        .dwLayerMask = 0,
        .dwVisibleMask = 0,
        .dwDamageMask = 0
    };

    State.Window.DeviceContext = GetDC(State.Window.HWND);

    if (State.Window.DeviceContext == NULL)
    {
        Exit("Unable to get device context.");
        return FALSE;
    }

    State.Window.PixelFormat = ChoosePixelFormat(State.Window.DeviceContext, &pfd);

    DescribePixelFormat(State.Window.DeviceContext, State.Window.PixelFormat, sizeof(pfd), &pfd);

    if (pfd.dwFlags & PFD_GENERIC_FORMAT)
    {
        Exit("OpenGL acceleration is not supported.");
        return FALSE;
    }

    if (!SetPixelFormat(State.Window.DeviceContext, State.Window.PixelFormat, &pfd))
    {
        Exit("Unable to set pixel format.");
        return FALSE;
    }

    State.Window.RenderContext = wglCreateContext(State.Window.DeviceContext);

    if (State.Window.RenderContext == NULL)
    {
        Exit("Unable to create rendering context.");
        return FALSE;
    }

    wglMakeCurrent(State.Window.DeviceContext, State.Window.RenderContext);

    if (!State.Window.IsWindowMode)
    {
        ShowCursor(FALSE);
    }

    GLInitialize();

    Clear(0, 0);
    ClearZBuffer();
    Toggle();

    return TRUE;
}

extern "C" u32 __cdecl CreateGameWindow(const s32, const s32, const s32, const s32, const HWND)
{
    return FALSE;
}

extern "C" u32 __cdecl DestroyGameWindow(const s32)
{
    return FALSE;
}

extern "C" u32 __cdecl SelectGameWindow(const s32, const s32)
{
    return FALSE;
}

extern "C" u32 __cdecl SetGameWindowMode(const s32 mode)
{
    State.Window.IsWindowMode = mode;

    return ChangeWindowMode();
}

extern "C" u32 __cdecl SetVideoMode(const s32 width, const s32 height, const s32 bpp, const s32, const s32)
{
    State.Window.Width = width;
    State.Window.Height = height;
    State.Window.BitsPerPixel = bpp;

    return ChangeWindowMode();
}

extern "C" u32 __cdecl RestoreVideoMode(void)
{
    GLRelease();

    if (!State.Window.IsWindowMode)
    {
        if (ChangeDisplaySettingsA(NULL, 0))
        {
            Exit("Unable to change display settings.");
            return FALSE;
        }
    }

    ShowCursor(TRUE);

    return TRUE;
}

extern "C" u32 __cdecl VideoRestore(void)
{
    if (State.Window.IsWindowMode) { return TRUE; }

    DEVMODEA dm = { 0 };

    dm.dmSize = sizeof(dm);
    dm.dmPelsWidth = State.Window.Width;
    dm.dmPelsHeight = State.Window.Height;
    dm.dmBitsPerPel = State.Window.BitsPerPixel;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

    if (ChangeDisplaySettingsA(&dm, CDS_FULLSCREEN))
    {
        Exit("Unable to change display settings.");
        return FALSE;
    }

    return TRUE;
}

extern "C" u32 __cdecl VideoSave(void)
{
    ResetTextureCache();

    if (State.Window.IsWindowMode) { return TRUE; }

    ShowWindow(State.Window.HWND, SW_MINIMIZE);

    if (ChangeDisplaySettingsA(NULL, 0))
    {
        Exit("Unable to change display settings.");
        return FALSE;
    }

    return TRUE;
}