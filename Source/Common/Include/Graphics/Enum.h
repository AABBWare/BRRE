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

#include "Basic.h"

namespace Renderer
{
    namespace Graphics
    {
        enum class TextureFormat : s32
        {
            Palette24Bit = 1,
            OpacityMap8Bit = 2,
            Color32Bit = 3,
            Color16Bit565 = 4,
            Color16Bit4444 = 5,
            PS22Palette24Bit = 6,
            PS2Color32Bit = 7,
            Palette32Bit = 8,
            PS2Palette32Bit = 9,
            PS2Palette4Bit = 10,
            GameCubeCompressed = 11,
            XBoxDXT1 = 12,
            XBoxColor8Bit256Colors = 13,
            XBoxColor8BitPalette32Bit = 14,
            XBoxTrueColor = 15
        };

        enum class TextureBlendOperation : s32
        {
            Negative = -1,
            None = 0,
            Disable = 1,
            SelectArgument1 = 2,
            SelectArgument2 = 3,
            Modulate = 4,
            Modulate2X = 5,
            Modulate4X = 6,
            Add = 7,
            AddSigned = 8,
            AddSigned2X = 9,
            Subtract = 10,
            AddSmooth = 11,
            BlendDiffuseAlpha = 12,
            BlendTextureAlpha = 13,
            BlendFactorAlpha = 14,
            BlendTextureAlphaPreMultiplied = 15,
            BlendCurrentAlpha = 16,
            PreModulate = 17,
            ModulateAlphaAddColor = 18,
            ModulateColorAddAlpha = 19,
            ModulateInverseAlphaAddColor = 20,
            ModulateInverseColorAddAlpha = 21,
            BumpEnvMap = 22,
            BumpEnvMapLuminance = 23,
            DotProduct3 = 24,
            MultiplyAdd = 25,
            Lerp = 26
        };

        enum class TextureSourceBlend : s32
        {
            Alpha = 0,
            One = 1,
            Zero = 2
        };

        enum class TextureDestinationBlend : s32
        {
            InverseAlpha = 0,
            One = 1,
            Color = 2,
            InverseColor = 3,
            Zero = 4,
            NegativeOne = 5
        };

        enum class CullMode : s32
        {
            CCW = 0,
            CW = 1,
            None = 2
        };

        enum class StencilPass : s32
        {
            LessEqualDecrement = 0,
            LessEqualIncrement = 1,
            GreaterEqualIncrement = 2,
            GreaterEqualDecrement = 3
        };

        enum class StencilFunction : s32
        {
            LessEqual = 0,
            Always = 1
        };

        enum class TextureClamp : s32
        {
            Wrap = 0,
            Clamp = 1
        };

        enum class ClippingMode : s32
        {
            Disabled = 0,
            Active = 31
        };
    }
}