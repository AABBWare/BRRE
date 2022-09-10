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

extern "C" u32 __cdecl SetDestBlend(const TextureDestinationBlend mode)
{
    switch (mode)
    {
        case TextureDestinationBlend::InverseAlpha:
        {
            State.GL.Mode.DestinationBlend = GL_ONE_MINUS_SRC_ALPHA;
            break;
        }
        case TextureDestinationBlend::One:
        {
            State.GL.Mode.DestinationBlend = GL_ONE;
            break;
        }
        case TextureDestinationBlend::Color:
        {
            State.GL.Mode.DestinationBlend = GL_SRC_COLOR;
            break;
        }
        case TextureDestinationBlend::InverseColor:
        {
            State.GL.Mode.DestinationBlend = GL_ONE_MINUS_SRC_COLOR;
            break;
        }
        case TextureDestinationBlend::Zero:
        {
            State.GL.Mode.DestinationBlend = GL_ZERO;
            break;
        }
    }

    return TRUE;
}

extern "C" u32 __cdecl SetMultiTextureBlend(const TextureBlendOperation operation)
{
    State.GL.Textures.MultiBlendOperation = operation;

    return TRUE;
}

extern "C" u32 __cdecl SetSrcBlend(const TextureSourceBlend mode)
{
    switch (mode)
    {
        case TextureSourceBlend::Alpha:
        {
            State.GL.Mode.SourceBlend = GL_SRC_ALPHA;
            break;
        }
        case TextureSourceBlend::One:
        {
            State.GL.Mode.SourceBlend = GL_ONE;
            break;
        }
        case TextureSourceBlend::Zero:
        {
            State.GL.Mode.SourceBlend = GL_ZERO;
            break;
        }
    }

    return TRUE;
}