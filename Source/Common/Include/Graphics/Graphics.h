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

#pragma once

#define BITS_PER_PIXEL_16 16
#define BITS_PER_PIXEL_24 24
#define BITS_PER_PIXEL_32 32

#define MAX_LIGHT_COUNT 3

// todo: move below into an enum
// todo: names
#define GRAPHICS_MODE_IS_TEXTURE_BLENDING_ENABLED 0x1
#define GRAPHICS_MODE_UNKNOWN_2 0x2
#define GRAPHICS_MODE_UNKNOWN_4 0x4
#define GRAPHICS_MODE_IS_FOG_ENABLED 0x8
#define GRAPHICS_MODE_IS_ALPHA_BLEND_ENABLED 0x20
#define GRAPHICS_MODE_IS_DEPTH_BUFFER_ENABLED 0x40
#define GRAPHICS_MODE_IS_DEPTH_BUFFER_WRITES_ENABLED 0x80
#define GRAPHICS_MODE_UNKNOWN_100 0x100
#define GRAPHICS_MODE_UNKNOWN_200 0x200
#define GRAPHICS_MODE_UNKNOWN_400 0x400

#define GRAPHICS_RENDERER_NAME_DIRECTX_8_1 "tridx8tl.dll"
#define GRAPHICS_RENDERER_NAME_OPENGL_1_2 "trigl.dll"