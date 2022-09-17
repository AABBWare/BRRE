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
        extern "C" BOOL __cdecl AllocateVertexBuffer(const void* vertexes, const u32 vertexCount, const u32 vertexSize, void** buffer)
        {
            if (State.Settings.UseFixedFunctionPipe != 0) { return FALSE; }

            auto bufferSize = vertexCount * vertexSize;

            IDirect3DVertexBuffer8* vertexBuffer;
            DX::DXC(State.DX.DirectXDevice->CreateVertexBuffer(bufferSize, D3DUSAGE_WRITEONLY, D3DFVF_TEX0,
                D3DPOOL::D3DPOOL_MANAGED, &vertexBuffer), "Unable to create vertex buffer.");

            void* data;
            DX::DXC(vertexBuffer->Lock(0, bufferSize, (BYTE**)&data, 0), "Unable to lock vertex buffer.");

            CopyMemory(data, vertexes, bufferSize);

            DX::DXC(vertexBuffer->Unlock(), "Unable to unlock vertex buffer.");

            *buffer = vertexBuffer;

            return TRUE;
        }

        extern "C" BOOL __cdecl FreeVertexBuffer(const void* buffer)
        {
            auto vertexBuffer = (IDirect3DVertexBuffer8*)buffer;

            if (vertexBuffer != NULL)
            {
                vertexBuffer->Release();
            }

            return TRUE;
        }
    }
}