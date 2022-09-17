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

#include "Module.h"
#include "State.h"

namespace Renderer
{
    namespace External
    {
        extern "C" BOOL __cdecl LockFrame(void)
        {
            if (State.DX.IsSceneActive)
            {
                EndScene();
            }

            if (!State.DX.IsFrameLocked)
            {
                State.DX.DirectXDevice->GetRenderTarget(&State.DX.Surfaces.DrawSurface);

                D3DLOCKED_RECT lr;
                if (State.DX.Surfaces.DrawSurface->LockRect(&lr, NULL, D3DLOCK_NOSYSLOCK) == 0)
                {
                    // todo unknown 4

                    State.DX.IsFrameLocked = TRUE;
                    return TRUE;
                }

                return FALSE;
            }

            return TRUE;
        }

        extern "C" BOOL __cdecl UnlockFrame(void)
        {
            if (!State.DX.IsFrameLocked) { return FALSE; }

            State.DX.IsFrameLocked = FALSE;

            // todo: unk4

            DX::DXC(State.DX.Surfaces.DrawSurface->UnlockRect(), "Unable to unlock back buffer.");

            State.DX.Surfaces.DrawSurface->Release();

            return TRUE;
        }
    }
}