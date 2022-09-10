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

#include "DXVertexShaderConstants.h"

const char DX_VERTEX_SHADER_NAME_DIRLIGHT[] = "Dirlight";
const DWORD DX_VERTEX_SHADER_FUNC_DIRLIGHT[1332]
{
    0xFFFE0101, 0x5FFFE, 0x454C4946, 0x6C726964,
    0x74686769, 0x6873762E, 0x72696400, 0x2FFFE,
    0x454E494C, 0x1, 0x2FFFE, 0x454E494C,
    0x2, 0x2FFFE, 0x454E494C, 0x3,
    0x2FFFE, 0x454E494C, 0x4, 0x2FFFE,
    0x454E494C, 0x5, 0x2FFFE, 0x454E494C,
    0x5, 0x4FFFE, 0x454C4946, 0x64616873,
    0x692E7265, 0x636E, 0x2FFFE, 0x454E494C,
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
    0x15, 0x2FFFE, 0x454E494C, 0x16,
    0x2FFFE, 0x454E494C, 0x17, 0x2FFFE,
    0x454E494C, 0x18, 0x2FFFE, 0x454E494C,
    0x19, 0x2FFFE, 0x454E494C, 0x1A,
    0x2FFFE, 0x454E494C, 0x1B, 0x2FFFE,
    0x454E494C, 0x1C, 0x2FFFE, 0x454E494C,
    0x1D, 0x2FFFE, 0x454E494C, 0x1E,
    0x2FFFE, 0x454E494C, 0x1F, 0x2FFFE,
    0x454E494C, 0x20, 0x2FFFE, 0x454E494C,
    0x21, 0x2FFFE, 0x454E494C, 0x22,
    0x2FFFE, 0x454E494C, 0x23, 0x2FFFE,
    0x454E494C, 0x24, 0x2FFFE, 0x454E494C,
    0x25, 0x2FFFE, 0x454E494C, 0x26,
    0x2FFFE, 0x454E494C, 0x27, 0x2FFFE,
    0x454E494C, 0x27, 0x2FFFE, 0x454E494C,
    0x28, 0x2FFFE, 0x454E494C, 0x28,
    0x2FFFE, 0x454E494C, 0x29, 0x2FFFE,
    0x454E494C, 0x29, 0x2FFFE, 0x454E494C,
    0x2A, 0x2FFFE, 0x454E494C, 0x2A,
    0x2FFFE, 0x454E494C, 0x2B, 0x2FFFE,
    0x454E494C, 0x2B, 0x2FFFE, 0x454E494C,
    0x2C, 0x2FFFE, 0x454E494C, 0x2C,
    0x2FFFE, 0x454E494C, 0x2D, 0x2FFFE,
    0x454E494C, 0x2D, 0x2FFFE, 0x454E494C,
    0x2E, 0x2FFFE, 0x454E494C, 0x2E,
    0x2FFFE, 0x454E494C, 0x2F, 0x2FFFE,
    0x454E494C, 0x30, 0x2FFFE, 0x454E494C,
    0x30, 0x2FFFE, 0x454E494C, 0x31,
    0x2FFFE, 0x454E494C, 0x31, 0x2FFFE,
    0x454E494C, 0x32, 0x2FFFE, 0x454E494C,
    0x33, 0x2FFFE, 0x454E494C, 0x33,
    0x2FFFE, 0x454E494C, 0x34, 0x2FFFE,
    0x454E494C, 0x34, 0x2FFFE, 0x454E494C,
    0x35, 0x2FFFE, 0x454E494C, 0x35,
    0x2FFFE, 0x454E494C, 0x36, 0x2FFFE,
    0x454E494C, 0x36, 0x2FFFE, 0x454E494C,
    0x37, 0x2FFFE, 0x454E494C, 0x37,
    0x2FFFE, 0x454E494C, 0x38, 0x2FFFE,
    0x454E494C, 0x38, 0x2FFFE, 0x454E494C,
    0x39, 0x2FFFE, 0x454E494C, 0x39,
    0x2FFFE, 0x454E494C, 0x3A, 0x2FFFE,
    0x454E494C, 0x3A, 0x2FFFE, 0x454E494C,
    0x3B, 0x2FFFE, 0x454E494C, 0x3B,
    0x2FFFE, 0x454E494C, 0x3C, 0x2FFFE,
    0x454E494C, 0x3C, 0x2FFFE, 0x454E494C,
    0x3D, 0x2FFFE, 0x454E494C, 0x3E,
    0x2FFFE, 0x454E494C, 0x3E, 0x2FFFE,
    0x454E494C, 0x3F, 0x2FFFE, 0x454E494C,
    0x3F, 0x2FFFE, 0x454E494C, 0x40,
    0x2FFFE, 0x454E494C, 0x40, 0x2FFFE,
    0x454E494C, 0x41, 0x2FFFE, 0x454E494C,
    0x41, 0x2FFFE, 0x454E494C, 0x42,
    0x2FFFE, 0x454E494C, 0x42, 0x2FFFE,
    0x454E494C, 0x43, 0x2FFFE, 0x454E494C,
    0x43, 0x2FFFE, 0x454E494C, 0x44,
    0x2FFFE, 0x454E494C, 0x44, 0x2FFFE,
    0x454E494C, 0x45, 0x2FFFE, 0x454E494C,
    0x45, 0x2FFFE, 0x454E494C, 0x46,
    0x2FFFE, 0x454E494C, 0x47, 0x2FFFE,
    0x454E494C, 0x47, 0x2FFFE, 0x454E494C,
    0x48, 0x2FFFE, 0x454E494C, 0x49,
    0x2FFFE, 0x454E494C, 0x4B, 0x2FFFE,
    0x454E494C, 0x4C, 0x2FFFE, 0x454E494C,
    0x4C, 0x2FFFE, 0x454E494C, 0x4D,
    0x2FFFE, 0x454E494C, 0x4D, 0x2FFFE,
    0x454E494C, 0x4E, 0x2FFFE, 0x454E494C,
    0x4F, 0x2FFFE, 0x454E494C, 0x50,
    0x2FFFE, 0x454E494C, 0x51, 0x2FFFE,
    0x454E494C, 0x52, 0x2FFFE, 0x454E494C,
    0x53, 0x2FFFE, 0x454E494C, 0x53,
    0x2FFFE, 0x454E494C, 0x54, 0x2FFFE,
    0x454E494C, 0x55, 0x2FFFE, 0x454E494C,
    0x56, 0x2FFFE, 0x454E494C, 0x57,
    0x2FFFE, 0x454E494C, 0x58, 0x2FFFE,
    0x454E494C, 0x59, 0x2FFFE, 0x454E494C,
    0x5A, 0x2FFFE, 0x454E494C, 0x5C,
    0x2FFFE, 0x454E494C, 0x5C, 0x2FFFE,
    0x454E494C, 0x5D, 0x2FFFE, 0x454E494C,
    0x5E, 0x2FFFE, 0x454E494C, 0x5F,
    0x2FFFE, 0x454E494C, 0x60, 0x2FFFE,
    0x454E494C, 0x62, 0x2FFFE, 0x454E494C,
    0x63, 0x2FFFE, 0x454E494C, 0x64,
    0x2FFFE, 0x454E494C, 0x64, 0x2FFFE,
    0x454E494C, 0x65, 0x2FFFE, 0x454E494C,
    0x66, 0x2FFFE, 0x454E494C, 0x67,
    0x2FFFE, 0x454E494C, 0x69, 0x2FFFE,
    0x454E494C, 0x69, 0x2FFFE, 0x454E494C,
    0x6A, 0x2FFFE, 0x454E494C, 0x6C,
    0x2FFFE, 0x454E494C, 0x6D, 0x2FFFE,
    0x454E494C, 0x6E, 0x2FFFE, 0x454E494C,
    0x6F, 0x2FFFE, 0x454E494C, 0x70,
    0x2FFFE, 0x454E494C, 0x71, 0x2FFFE,
    0x454E494C, 0x71, 0x2FFFE, 0x454E494C,
    0x72, 0x2FFFE, 0x454E494C, 0x73,
    0x2FFFE, 0x454E494C, 0x74, 0x2FFFE,
    0x454E494C, 0x75, 0x2FFFE, 0x454E494C,
    0x76, 0x2FFFE, 0x454E494C, 0x77,
    0x2FFFE, 0x454E494C, 0x78, 0x2FFFE,
    0x454E494C, 0x79, 0x2FFFE, 0x454E494C,
    0x7A, 0x2FFFE, 0x454E494C, 0x7B,
    0x2FFFE, 0x454E494C, 0x7D, 0x2FFFE,
    0x454E494C, 0x7E, 0x2FFFE, 0x454E494C,
    0x7E, 0x2FFFE, 0x454E494C, 0x7F,
    0x2FFFE, 0x454E494C, 0x80, 0x2FFFE,
    0x454E494C, 0x81, 0x2FFFE, 0x454E494C,
    0x82, 0x2FFFE, 0x454E494C, 0x83,
    0x2FFFE, 0x454E494C, 0x84, 0x2FFFE,
    0x454E494C, 0x85, 0x2FFFE, 0x454E494C,
    0x86, 0x2FFFE, 0x454E494C, 0x87,
    0x2FFFE, 0x454E494C, 0x88, 0x2FFFE,
    0x454E494C, 0x89, 0x2FFFE, 0x454E494C,
    0x8A, 0x2FFFE, 0x454E494C, 0x8B,
    0x2FFFE, 0x454E494C, 0x8C, 0x2FFFE,
    0x454E494C, 0x8D, 0x2FFFE, 0x454E494C,
    0x8F, 0x2FFFE, 0x454E494C, 0x90,
    0x2FFFE, 0x454E494C, 0x91, 0x2FFFE,
    0x454E494C, 0x92, 0x2FFFE, 0x454E494C,
    0x92, 0x2FFFE, 0x454E494C, 0x93,
    0x2FFFE, 0x454E494C, 0x94, 0x2FFFE,
    0x454E494C, 0x95, 0x2FFFE, 0x454E494C,
    0x96, 0x2FFFE, 0x454E494C, 0x97,
    0x2FFFE, 0x454E494C, 0x97, 0x2FFFE,
    0x454E494C, 0x98, 0x2FFFE, 0x454E494C,
    0x99, 0x2FFFE, 0x454E494C, 0x9A,
    0x2FFFE, 0x454E494C, 0x9B, 0x2FFFE,
    0x454E494C, 0x9C, 0x2FFFE, 0x454E494C,
    0x9D, 0x2FFFE, 0x454E494C, 0x9E,
    0x2FFFE, 0x454E494C, 0x9F, 0x2FFFE,
    0x454E494C, 0xA0, 0x2FFFE, 0x454E494C,
    0xA1, 0x2FFFE, 0x454E494C, 0xA2,
    0x2FFFE, 0x454E494C, 0xA3, 0x2FFFE,
    0x454E494C, 0xA4, 0x2FFFE, 0x454E494C,
    0xA5, 0x2FFFE, 0x454E494C, 0xA6,
    0x2FFFE, 0x454E494C, 0xA7, 0x2FFFE,
    0x454E494C, 0xA8, 0x2FFFE, 0x454E494C,
    0xA9, 0x2FFFE, 0x454E494C, 0xAA,
    0x2FFFE, 0x454E494C, 0xAB, 0x2FFFE,
    0x454E494C, 0xAC, 0x2FFFE, 0x454E494C,
    0xAD, 0x2FFFE, 0x454E494C, 0xAE,
    0x2FFFE, 0x454E494C, 0xAF, 0x2FFFE,
    0x454E494C, 0xB0, 0x2FFFE, 0x454E494C,
    0xB1, 0x2FFFE, 0x454E494C, 0xB2,
    0x2FFFE, 0x454E494C, 0xB3, 0x2FFFE,
    0x454E494C, 0xB4, 0x2FFFE, 0x454E494C,
    0xB5, 0x2FFFE, 0x454E494C, 0xB7,
    0x2FFFE, 0x454E494C, 0xB8, 0x2FFFE,
    0x454E494C, 0xB9, 0x2FFFE, 0x454E494C,
    0xBA, 0x2FFFE, 0x454E494C, 0xBA,
    0x2FFFE, 0x454E494C, 0xBB, 0x2FFFE,
    0x454E494C, 0xBD, 0x2FFFE, 0x454E494C,
    0xBE, 0x2FFFE, 0x454E494C, 0xBE,
    0x2FFFE, 0x454E494C, 0xBF, 0x2FFFE,
    0x454E494C, 0xC1, 0x2FFFE, 0x454E494C,
    0xC2, 0x2FFFE, 0x454E494C, 0xC2,
    0x2FFFE, 0x454E494C, 0xC3, 0x2FFFE,
    0x454E494C, 0xC5, 0x2FFFE, 0x454E494C,
    0xC6, 0x2FFFE, 0x454E494C, 0xC6,
    0x2FFFE, 0x454E494C, 0xC7, 0x2FFFE,
    0x454E494C, 0xC9, 0x2FFFE, 0x454E494C,
    0xCA, 0x2FFFE, 0x454E494C, 0xCB,
    0x2FFFE, 0x454E494C, 0xCC, 0x2FFFE,
    0x454E494C, 0xCD, 0x2FFFE, 0x454E494C,
    0xCE, 0x2FFFE, 0x454E494C, 0xCF,
    0x2FFFE, 0x454E494C, 0xD0, 0x2FFFE,
    0x454E494C, 0xD1, 0x2FFFE, 0x454E494C,
    0xD1, 0x2FFFE, 0x454E494C, 0xD2,
    0x2FFFE, 0x454E494C, 0xD3, 0x2FFFE,
    0x454E494C, 0xD4, 0x2FFFE, 0x454E494C,
    0xD5, 0x2FFFE, 0x454E494C, 0xD6,
    0x2FFFE, 0x454E494C, 0xD7, 0x2FFFE,
    0x454E494C, 0xD8, 0x2FFFE, 0x454E494C,
    0xD9, 0x2FFFE, 0x454E494C, 0xDA,
    0x2FFFE, 0x454E494C, 0xDC, 0x2FFFE,
    0x454E494C, 0xDD, 0x2FFFE, 0x454E494C,
    0xDD, 0x2FFFE, 0x454E494C, 0xDE,
    0x2FFFE, 0x454E494C, 0xDF, 0x2FFFE,
    0x454E494C, 0xE0, 0x2FFFE, 0x454E494C,
    0xE1, 0x2FFFE, 0x454E494C, 0xE2,
    0x2FFFE, 0x454E494C, 0xE3, 0x2FFFE,
    0x454E494C, 0xE4, 0x2FFFE, 0x454E494C,
    0xE5, 0x2FFFE, 0x454E494C, 0xE6,
    0x2FFFE, 0x454E494C, 0xE7, 0x2FFFE,
    0x454E494C, 0xE8, 0x2FFFE, 0x454E494C,
    0xE9, 0x2FFFE, 0x454E494C, 0xEA,
    0x2FFFE, 0x454E494C, 0xEB, 0x2FFFE,
    0x454E494C, 0xEC, 0x2FFFE, 0x454E494C,
    0xED, 0x2FFFE, 0x454E494C, 0xEE,
    0x2FFFE, 0x454E494C, 0xEF, 0x2FFFE,
    0x454E494C, 0xF0, 0x2FFFE, 0x454E494C,
    0xF1, 0x2FFFE, 0x454E494C, 0xF2,
    0x2FFFE, 0x454E494C, 0xF3, 0x2FFFE,
    0x454E494C, 0xF4, 0x2FFFE, 0x454E494C,
    0xF5, 0x2FFFE, 0x454E494C, 0xF6,
    0x2FFFE, 0x454E494C, 0xF6, 0x2FFFE,
    0x454E494C, 0xF7, 0x2FFFE, 0x454E494C,
    0xF8, 0x2FFFE, 0x454E494C, 0xF9,
    0x2FFFE, 0x454E494C, 0xFA, 0x2FFFE,
    0x454E494C, 0xFB, 0x2FFFE, 0x454E494C,
    0xFC, 0x2FFFE, 0x454E494C, 0xFD,
    0x2FFFE, 0x454E494C, 0xFE, 0x2FFFE,
    0x454E494C, 0xFF, 0x2FFFE, 0x454E494C,
    0x100, 0x2FFFE, 0x454E494C, 0x102,
    0x2FFFE, 0x454E494C, 0x103, 0x2FFFE,
    0x454E494C, 0x103, 0x2FFFE, 0x454E494C,
    0x104, 0x2FFFE, 0x454E494C, 0x105,
    0x2FFFE, 0x454E494C, 0x106, 0x2FFFE,
    0x454E494C, 0x107, 0x2FFFE, 0x454E494C,
    0x108, 0x2FFFE, 0x454E494C, 0x109,
    0x2FFFE, 0x454E494C, 0x10A, 0x2FFFE,
    0x454E494C, 0x10B, 0x2FFFE, 0x454E494C,
    0x10C, 0x2FFFE, 0x454E494C, 0x10D,
    0x2FFFE, 0x454E494C, 0x10E, 0x2FFFE,
    0x454E494C, 0x10F, 0x2FFFE, 0x454E494C,
    0x110, 0x2FFFE, 0x454E494C, 0x111,
    0x2FFFE, 0x454E494C, 0x112, 0x2FFFE,
    0x454E494C, 0x112, 0x2FFFE, 0x454E494C,
    0x113, 0x2FFFE, 0x454E494C, 0x114,
    0x2FFFE, 0x454E494C, 0x115, 0x2FFFE,
    0x454E494C, 0x116, 0x2FFFE, 0x454E494C,
    0x117, 0x2FFFE, 0x454E494C, 0x118,
    0x2FFFE, 0x454E494C, 0x119, 0x2FFFE,
    0x454E494C, 0x11A, 0x2FFFE, 0x454E494C,
    0x11C, 0x2FFFE, 0x454E494C, 0x11C,
    0x2FFFE, 0x454E494C, 0x11D, 0x2FFFE,
    0x454E494C, 0x11E, 0x2FFFE, 0x454E494C,
    0x11F, 0x2FFFE, 0x454E494C, 0x120,
    0x2FFFE, 0x454E494C, 0x121, 0x2FFFE,
    0x454E494C, 0x123, 0x2FFFE, 0x454E494C,
    0x124, 0x2FFFE, 0x454E494C, 0x125,
    0x2FFFE, 0x454E494C, 0x126, 0x2FFFE,
    0x454E494C, 0x127, 0x2FFFE, 0x454E494C,
    0x128, 0x2FFFE, 0x454E494C, 0x129,
    0x2FFFE, 0x454E494C, 0x12A, 0x2FFFE,
    0x454E494C, 0x12B, 0x2FFFE, 0x454E494C,
    0x12C, 0x2FFFE, 0x454E494C, 0x12D,
    0x2FFFE, 0x454E494C, 0x12D, 0x2FFFE,
    0x454E494C, 0x12E, 0x2FFFE, 0x454E494C,
    0x12F, 0x2FFFE, 0x454E494C, 0x130,
    0x2FFFE, 0x454E494C, 0x131, 0x2FFFE,
    0x454E494C, 0x132, 0x2FFFE, 0x454E494C,
    0x133, 0x2FFFE, 0x454E494C, 0x134,
    0x2FFFE, 0x454E494C, 0x135, 0x2FFFE,
    0x454E494C, 0x135, 0x2FFFE, 0x454E494C,
    0x136, 0x2FFFE, 0x454E494C, 0x137,
    0x2FFFE, 0x454E494C, 0x138, 0x2FFFE,
    0x454E494C, 0x139, 0x2FFFE, 0x454E494C,
    0x13A, 0x2FFFE, 0x454E494C, 0x13B,
    0x2FFFE, 0x454E494C, 0x13C, 0x2FFFE,
    0x454E494C, 0x13D, 0x2FFFE, 0x454E494C,
    0x13E, 0x2FFFE, 0x454E494C, 0x13F,
    0x2FFFE, 0x454E494C, 0x140, 0x2FFFE,
    0x454E494C, 0x142, 0x2FFFE, 0x454E494C,
    0x143, 0x2FFFE, 0x454E494C, 0x144,
    0x2FFFE, 0x454E494C, 0x145, 0x2FFFE,
    0x454E494C, 0x146, 0x2FFFE, 0x454E494C,
    0x146, 0x2FFFE, 0x454E494C, 0x147,
    0x2FFFE, 0x454E494C, 0x148, 0x2FFFE,
    0x454E494C, 0x14A, 0x2FFFE, 0x454E494C,
    0x14B, 0x2FFFE, 0x454E494C, 0x14C,
    0x2FFFE, 0x454E494C, 0x14D, 0x2FFFE,
    0x454E494C, 0x14D, 0x2FFFE, 0x454E494C,
    0x14E, 0x2FFFE, 0x454E494C, 0x14F,
    0x2FFFE, 0x454E494C, 0x150, 0x2FFFE,
    0x454E494C, 0x152, 0x2FFFE, 0x454E494C,
    0x153, 0x5FFFE, 0x454C4946, 0x6C726964,
    0x74686769, 0x6873762E, 0x72696400, 0x2FFFE,
    0x454E494C, 0x6, 0x2FFFE, 0x454E494C,
    0x7, 0x2FFFE, 0x454E494C, 0x4D,
    0x2FFFE, 0x454E494C, 0x8, 0x2FFFE,
    0x454E494C, 0x9, 0x2FFFE, 0x454E494C,
    0x5C, 0x9, 0xC0010000, 0x90E40000,
    0xA0E40000, 0x2FFFE, 0x454E494C, 0x5E,
    0x9, 0xC0020000, 0x90E40000, 0xA0E40001,
    0x2FFFE, 0x454E494C, 0x5F, 0x9,
    0xC0040000, 0x90E40000, 0xA0E40002, 0x2FFFE,
    0x454E494C, 0x60, 0x9, 0xC0080000,
    0x90E40000, 0xA0E40003, 0x2FFFE, 0x454E494C,
    0xA, 0x2FFFE, 0x454E494C, 0x71,
    0x8, 0x80010002, 0x90E40003, 0xA0E40004,
    0x2FFFE, 0x454E494C, 0x73, 0x8,
    0x80020002, 0x90E40003, 0xA0E40014, 0x2FFFE,
    0x454E494C, 0x74, 0x8, 0x80040002,
    0x90E40003, 0xA0E40016, 0x2FFFE, 0x454E494C,
    0x75, 0x1, 0x80080002, 0xA0E40007,
    0x2FFFE, 0x454E494C, 0x76, 0xB,
    0x800F0002, 0x80E40002, 0xA0E40018, 0x2FFFE,
    0x454E494C, 0x77, 0x9, 0x80010001,
    0x80E40002, 0xA0E40008, 0x2FFFE, 0x454E494C,
    0x78, 0x9, 0x80020001, 0x80E40002,
    0xA0E40015, 0x2FFFE, 0x454E494C, 0x79,
    0x9, 0x80040001, 0x80E40002, 0xA0E40017,
    0x2FFFE, 0x454E494C, 0x7A, 0x1,
    0x80080001, 0xA0FF0005, 0x2FFFE, 0x454E494C,
    0x7B, 0x1, 0xD00F0000, 0x80E40001,
    0x2FFFE, 0x454E494C, 0xB, 0x2FFFE,
    0x454E494C, 0xBA, 0x1, 0xE00F0000,
    0x90E40007, 0x2FFFE, 0x454E494C, 0xC,
    0x2FFFE, 0x454E494C, 0xDD, 0x2FFFE,
    0x454E494C, 0xDE, 0x9, 0x80040000,
    0x90E40000, 0xA0E40002, 0x2FFFE, 0x454E494C,
    0xDF, 0x2, 0x80040000, 0x80AA0000,
    0xA1000006, 0x2FFFE, 0x454E494C, 0xE0,
    0x5, 0x80040000, 0x80AA0000, 0xA0550006,
    0x2FFFE, 0x454E494C, 0xE1, 0xB,
    0x80040000, 0x80AA0000, 0xA0000005, 0x2FFFE,
    0x454E494C, 0xE2, 0xA, 0x80040000,
    0x80AA0000, 0xA0550005, 0x2FFFE, 0x454E494C,
    0xE3, 0x2, 0xC0010001, 0xA0550005,
    0x81AA0000, 0x2FFFE, 0x454E494C, 0xD,
    0x2FFFE, 0x454E494C, 0xE, 0xFFFF
};