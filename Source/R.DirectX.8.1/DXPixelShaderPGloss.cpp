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

#include "DXPixelShaderConstants.h"

const char DX_PIXEL_SHADER_NAME_PGLOSS[] = "Pgloss";
const DWORD DX_PIXEL_SHADER_FUNC_PGLOSS[101]
{
    0xFFFF0101, 0x4FFFE, 0x454C4946, 0x6F6C6770,
    0x702E7373, 0x70006873, 0x2FFFE, 0x454E494C,
    0x1, 0x2FFFE, 0x454E494C, 0x2,
    0x2FFFE, 0x454E494C, 0x3, 0x2FFFE,
    0x454E494C, 0x4, 0x2FFFE, 0x454E494C,
    0x5, 0x2FFFE, 0x454E494C, 0x6,
    0x2FFFE, 0x454E494C, 0x7, 0x2FFFE,
    0x454E494C, 0x7, 0x42, 0xB00F0000,
    0x2FFFE, 0x454E494C, 0x8, 0x2FFFE,
    0x454E494C, 0x8, 0x42, 0xB00F0001,
    0x2FFFE, 0x454E494C, 0x9, 0x2FFFE,
    0x454E494C, 0x9, 0x42, 0xB00F0002,
    0x2FFFE, 0x454E494C, 0xA, 0x2FFFE,
    0x454E494C, 0xB, 0x2FFFE, 0x454E494C,
    0xB, 0x1, 0x800F0000, 0xB0E40000,
    0x2FFFE, 0x454E494C, 0xC, 0x2FFFE,
    0x454E494C, 0xD, 0x2FFFE, 0x454E494C,
    0xE, 0x2FFFE, 0x454E494C, 0xF,
    0x2FFFE, 0x454E494C, 0xF, 0x2FFFE,
    0x454E494C, 0x10, 0x2FFFE, 0x454E494C,
    0x11, 0x2FFFE, 0x454E494C, 0x13,
    0x2FFFE, 0x454E494C, 0x14, 0x2FFFE,
    0x454E494C, 0x14, 0x5, 0x800F0000,
    0x80E40000, 0x90E40000, 0x2FFFE, 0x454E494C,
    0x15, 0x2FFFE, 0x454E494C, 0x16,
    0xFFFF
};