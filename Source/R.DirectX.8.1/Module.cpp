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

#define UNKNOWN_DEVICE_NAME "Unknown Device"

extern "C" u32 __cdecl Init(const HWND hwnd, const struct InitializeModuleArguments* args)
{
    Kill();

    State.Window.HWND = hwnd;

    for (auto x = 0; x < MAX_WINDOW_COUNT; x++)
    {
        State.DX.Windows.Windows[x] = NULL;
    }

    auto dxw = new DXWindow();

    if (dxw == NULL)
    {
        Exit("Unable to allocate memory for DirectX window.");
        return FALSE;
    }

    DXWindowConstructor(dxw);

    State.DX.Windows.Windows[0] = dxw;
    State.DX.Windows.Current = dxw;

    auto dx = Direct3DCreate8(D3D_SDK_VERSION);

    if (dx == NULL)
    {
        Exit("Unable to initialize DirectX.");
        return FALSE;
    }

    State.DX.DirectX = dx;
    State.DX.AvailableDevices.Count = State.DX.DirectX->GetAdapterCount();

    for (auto x = 0; x < State.DX.AvailableDevices.Count; x++)
    {
        D3DADAPTER_IDENTIFIER8 identifier;
        auto result = State.DX.DirectX->GetAdapterIdentifier(x, D3DENUM_NO_WHQL_LEVEL, &identifier);

        State.DX.AvailableDevices.Details[x].Index = x;

        ZeroMemory(State.DX.AvailableDevices.Details[x].Driver, MAX_DEVICE_IDENTIFIER_STRING);
        ZeroMemory(State.DX.AvailableDevices.Details[x].Description, MAX_DEVICE_IDENTIFIER_STRING);

        if (result == 0)
        {
            State.DX.AvailableDevices.Details[x].DeviceID = identifier.DeviceId;
            State.DX.AvailableDevices.Details[x].SubSystemID = identifier.SubSysId;
            CopyMemory(State.DX.AvailableDevices.Details[x].Driver, identifier.Driver, MAX_DEVICE_IDENTIFIER_STRING);
            CopyMemory(State.DX.AvailableDevices.Details[x].Description, identifier.Description, MAX_DEVICE_IDENTIFIER_STRING);
        }
        else
        {
            State.DX.AvailableDevices.Details[x].DeviceID = 0;
            State.DX.AvailableDevices.Details[x].SubSystemID = 0;
            CopyMemory(State.DX.AvailableDevices.Details[x].Driver, UNKNOWN_DEVICE_NAME, strlen(UNKNOWN_DEVICE_NAME));
            CopyMemory(State.DX.AvailableDevices.Details[x].Description, UNKNOWN_DEVICE_NAME, strlen(UNKNOWN_DEVICE_NAME));
        }
    }

    State.Window.HWND = hwnd;
    State.DX.Windows.Current->HWND = hwnd;

    CopyMemory(&State.InitializeArguments, args, sizeof(struct InitializeModuleArguments));

    State.DX.DirectX->Release();
    State.DX.DirectX = NULL;

    return TRUE;
}

extern "C" u32 __cdecl Kill(void)
{
    DXDestroy();

    for (auto x = 0; x < MAX_WINDOW_COUNT; x++)
    {
        if (State.DX.Windows.Windows[x] != NULL)
        {
            delete State.DX.Windows.Windows[x];

            State.DX.Windows.Windows[x] = NULL;
        }
    }

    State.DX.Windows.Current = NULL;

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

    return TRUE;
}