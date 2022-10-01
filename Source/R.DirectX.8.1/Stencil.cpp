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

using namespace Renderer::Enums;

namespace Renderer::Module
{
    extern "C" BOOL __cdecl SetStencilEnable(const BOOL mode)
    {
        State.DX.IsStencilSet = mode;

        DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILENABLE, mode);
        DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZENABLE, D3DZBUFFERTYPE::D3DZB_TRUE);

        if (mode == 0)
        {
            DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZWRITEENABLE, TRUE);
            DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_LESSEQUAL);
        }
        else
        {
            DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZWRITEENABLE, FALSE);
        }

        return TRUE;
    }

    extern "C" BOOL __cdecl SetStencilFunc(const StencilFunction function)
    {
        auto value = function == StencilFunction::LessEqual
            ? D3DCMPFUNC::D3DCMP_LESSEQUAL
            : D3DCMPFUNC::D3DCMP_ALWAYS;

        DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILFUNC, value);

        return TRUE;
    }

    extern "C" BOOL __cdecl SetStencilPass(const StencilPass pass)
    {
        switch (pass)
        {
        case StencilPass::LessEqualDecrement:
        {
            DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILPASS, D3DSTENCILOP::D3DSTENCILOP_DECR);
            DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_LESSEQUAL);
            break;
        }
        case StencilPass::LessEqualIncrement:
        {
            DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILPASS, D3DSTENCILOP::D3DSTENCILOP_INCR);
            DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_LESSEQUAL);
            break;
        }
        case StencilPass::GreaterEqualIncrement:
        {
            DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILPASS, D3DSTENCILOP::D3DSTENCILOP_INCR);
            DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_GREATEREQUAL);
            break;
        }
        case StencilPass::GreaterEqualDecrement:
        {
            DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILPASS, D3DSTENCILOP::D3DSTENCILOP_DECR);
            DX::SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_GREATEREQUAL);
            break;
        }
        }

        return TRUE;
    }
}