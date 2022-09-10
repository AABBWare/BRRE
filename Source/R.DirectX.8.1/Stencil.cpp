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

extern "C" u32 __cdecl SetStencilEnable(const u32 mode)
{
    State.DX.IsStencilSet = mode;

    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILENABLE, mode);
    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZENABLE, D3DZBUFFERTYPE::D3DZB_TRUE);

    if (mode == 0)
    {
        DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZWRITEENABLE, TRUE);
        DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_LESSEQUAL);
    }
    else
    {
        DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZWRITEENABLE, FALSE);
    }

    return TRUE;
}

extern "C" u32 __cdecl SetStencilFunc(const StencilFunction function)
{
    auto value = function == StencilFunction::LessEqual
        ? D3DCMPFUNC::D3DCMP_LESSEQUAL
        : D3DCMPFUNC::D3DCMP_ALWAYS;

    DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILFUNC, value);

    return TRUE;
}

extern "C" u32 __cdecl SetStencilPass(const StencilPass pass)
{
    switch (pass)
    {
        case StencilPass::LessEqualDecrement:
        {
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILPASS, D3DSTENCILOP::D3DSTENCILOP_DECR);
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_LESSEQUAL);

            break;
        }
        case StencilPass::LessEqualIncrement:
        {
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILPASS, D3DSTENCILOP::D3DSTENCILOP_INCR);
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_LESSEQUAL);

            break;
        }
        case StencilPass::GreaterEqualIncrement:
        {
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILPASS, D3DSTENCILOP::D3DSTENCILOP_INCR);
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_GREATEREQUAL);
            break;
        }
        case StencilPass::GreaterEqualDecrement:
        {
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_STENCILPASS, D3DSTENCILOP::D3DSTENCILOP_DECR);
            DXSetRenderState(D3DRENDERSTATETYPE::D3DRS_ZFUNC, D3DCMPFUNC::D3DCMP_GREATEREQUAL);
            break;
        }
    }

    return TRUE;
}