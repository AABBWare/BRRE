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

namespace Renderer
{
    namespace External
    {
        namespace DX
        {
            namespace Shader
            {
                namespace Pixel
                {
                    const char NAME_BUMP[] = "Bump";
                    const DWORD FUNC_BUMP[238]
                    {
                        0xFFFF0101, 0x4FFFE, 0x454C4946, 0x706D7562,
                        0x6873702E, 0x6D756200, 0x2FFFE, 0x454E494C,
                        0x1, 0x2FFFE, 0x454E494C, 0x2,
                        0x2FFFE, 0x454E494C, 0x3, 0x2FFFE,
                        0x454E494C, 0x4, 0x2FFFE, 0x454E494C,
                        0x5, 0x2FFFE, 0x454E494C, 0x6,
                        0x2FFFE, 0x454E494C, 0x7, 0x2FFFE,
                        0x454E494C, 0x8, 0x2FFFE, 0x454E494C,
                        0x9, 0x2FFFE, 0x454E494C, 0xA,
                        0x2FFFE, 0x454E494C, 0xB, 0x2FFFE,
                        0x454E494C, 0xC, 0x2FFFE, 0x454E494C,
                        0xD, 0x2FFFE, 0x454E494C, 0xE,
                        0x2FFFE, 0x454E494C, 0xF, 0x2FFFE,
                        0x454E494C, 0x10, 0x2FFFE, 0x454E494C,
                        0x11, 0x2FFFE, 0x454E494C, 0x12,
                        0x2FFFE, 0x454E494C, 0x13, 0x2FFFE,
                        0x454E494C, 0x14, 0x2FFFE, 0x454E494C,
                        0x15, 0x2FFFE, 0x454E494C, 0x15,
                        0x2FFFE, 0x454E494C, 0x16, 0x2FFFE,
                        0x454E494C, 0x18, 0x2FFFE, 0x454E494C,
                        0x19, 0x2FFFE, 0x454E494C, 0x1A,
                        0x2FFFE, 0x454E494C, 0x1B, 0x2FFFE,
                        0x454E494C, 0x1C, 0x2FFFE, 0x454E494C,
                        0x1D, 0x2FFFE, 0x454E494C, 0x1D,
                        0x42, 0xB00F0000, 0x2FFFE, 0x454E494C,
                        0x1E, 0x2FFFE, 0x454E494C, 0x1E,
                        0x42, 0xB00F0001, 0x2FFFE, 0x454E494C,
                        0x1F, 0x2FFFE, 0x454E494C, 0x20,
                        0x2FFFE, 0x454E494C, 0x21, 0x2FFFE,
                        0x454E494C, 0x22, 0x2FFFE, 0x454E494C,
                        0x22, 0x2FFFE, 0x454E494C, 0x23,
                        0x2FFFE, 0x454E494C, 0x24, 0x2FFFE,
                        0x454E494C, 0x25, 0x2FFFE, 0x454E494C,
                        0x26, 0x2FFFE, 0x454E494C, 0x28,
                        0x2FFFE, 0x454E494C, 0x28, 0x8,
                        0x801F0000, 0xB4E40000, 0x94E40000, 0x2FFFE,
                        0x454E494C, 0x29, 0x2FFFE, 0x454E494C,
                        0x29, 0x5, 0x80070000, 0x80E40000,
                        0xA0E40001, 0x2FFFE, 0x454E494C, 0x2A,
                        0x2FFFE, 0x454E494C, 0x2A, 0x2,
                        0x800F0000, 0x80E40000, 0xA0E40000, 0x2FFFE,
                        0x454E494C, 0x2B, 0x2FFFE, 0x454E494C,
                        0x2B, 0x2, 0x80070000, 0x80E40000,
                        0x90E40001, 0x2FFFE, 0x454E494C, 0x2C,
                        0x2FFFE, 0x454E494C, 0x2D, 0x2FFFE,
                        0x454E494C, 0x2E, 0x2FFFE, 0x454E494C,
                        0x2F, 0x2FFFE, 0x454E494C, 0x30,
                        0x2FFFE, 0x454E494C, 0x30, 0x1,
                        0x80080000, 0x90FF0000, 0x2FFFE, 0x454E494C,
                        0x31, 0x2FFFE, 0x454E494C, 0x32,
                        0x2FFFE, 0x454E494C, 0x33, 0x2FFFE,
                        0x454E494C, 0x34, 0x2FFFE, 0x454E494C,
                        0x34, 0x5, 0x800F0000, 0x80E40000,
                        0xB0E40001, 0x2FFFE, 0x454E494C, 0x35,
                        0x2FFFE, 0x454E494C, 0x36, 0x2FFFE,
                        0x454E494C, 0x37, 0x2FFFE, 0x454E494C,
                        0x38, 0x2FFFE, 0x454E494C, 0x39,
                        0x2FFFE, 0x454E494C, 0x39, 0x2FFFE,
                        0x454E494C, 0x3A, 0x2FFFE, 0x454E494C,
                        0x3C, 0xFFFF
                    };
                }
            }
        }
    }
}