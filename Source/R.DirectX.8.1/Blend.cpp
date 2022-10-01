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
    extern "C" BOOL __cdecl SetDestinationBlend(const DestinationBlend mode)
    {
        State.DX.Mode.Blend.DestinationBlendOperation = D3DBLENDOP::D3DBLENDOP_ADD;

        switch (mode)
        {
        case DestinationBlend::InverseAlpha:
        {
            State.DX.Mode.Blend.DestinationBlend = D3DBLEND::D3DBLEND_INVSRCALPHA;
            State.DX.Mode.Blend.DestinationBlendOperation = D3DBLENDOP::D3DBLENDOP_ADD;
            break;
        }
        case DestinationBlend::One:
        {
            State.DX.Mode.Blend.DestinationBlend = D3DBLEND::D3DBLEND_ONE;
            State.DX.Mode.Blend.DestinationBlendOperation = D3DBLENDOP::D3DBLENDOP_ADD;
            break;
        }
        case DestinationBlend::Color:
        {
            State.DX.Mode.Blend.DestinationBlend = D3DBLEND::D3DBLEND_SRCCOLOR;
            State.DX.Mode.Blend.DestinationBlendOperation = D3DBLENDOP::D3DBLENDOP_ADD;
            break;
        }
        case DestinationBlend::InverseColor:
        {
            State.DX.Mode.Blend.DestinationBlend = D3DBLEND::D3DBLEND_INVSRCCOLOR;
            State.DX.Mode.Blend.DestinationBlendOperation = D3DBLENDOP::D3DBLENDOP_ADD;
            break;
        }
        case DestinationBlend::Zero:
        {
            State.DX.Mode.Blend.DestinationBlend = D3DBLEND::D3DBLEND_ZERO;
            State.DX.Mode.Blend.DestinationBlendOperation = D3DBLENDOP::D3DBLENDOP_ADD;
            break;
        }
        case DestinationBlend::NegativeOne:
        {
            State.DX.Mode.Blend.DestinationBlend = D3DBLEND::D3DBLEND_ONE;
            State.DX.Mode.Blend.DestinationBlendOperation = D3DBLENDOP::D3DBLENDOP_REVSUBTRACT;
            break;
        }
        }

        return TRUE;
    }

    extern "C" BOOL __cdecl SetMultiTextureBlend(const BlendOperation operation)
    {
        State.DX.Textures.BlendOperation = operation;

        if (operation == BlendOperation::None || operation == BlendOperation::SelectArgument2)
        {
            return TRUE;
        }

        // D3DTEXOPCAPS_MODULATEINVCOLOR_ADDALPHA  0x00100000L
        return (State.DX.CurrentDevice.Capabilities.DisplayCapabilities.TextureOpCaps >> 23) & 1;
    }

    extern "C" BOOL __cdecl SetSourceBlend(const SourceBlend mode)
    {
        switch (mode)
        {
        case SourceBlend::Alpha:
        {
            State.DX.Mode.Blend.SourceBlend = D3DBLEND::D3DBLEND_SRCALPHA;
            break;
        }
        case SourceBlend::One:
        {
            State.DX.Mode.Blend.SourceBlend = D3DBLEND::D3DBLEND_ONE;
            break;
        }
        case SourceBlend::Zero:
        {
            State.DX.Mode.Blend.SourceBlend = D3DBLEND::D3DBLEND_ZERO;
            break;
        }
        }

        return TRUE;
    }
}