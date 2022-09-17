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

#include "Exit.h"
#include "Module.h"
#include "State.h"

using namespace Renderer::Graphics;

namespace Renderer
{
    namespace External
    {
        extern "C" u32 __cdecl AllocateTexture(const struct Texture* texture)
        {
            u32 indx = 0;

            for (auto x = 1; x < MAX_TEXTURE_COUNT; x++)
            {
                if (State.DX.Textures.Textures[x].Texture == NULL)
                {
                    indx = x;
                    break;
                }
            }

            if (indx == 0 || MAX_TEXTURE_COUNT <= indx) { return 0; }

            FreeTexture(indx);

            auto dxt = &State.DX.Textures.Textures[indx];

            dxt->Width = texture->Width;
            dxt->Height = texture->Height;
            dxt->Options = texture->Options;
            dxt->MipLevelCount = texture->MipLevelCount;

            switch (texture->Format)
            {
            case TextureFormat::Palette24Bit:
            case TextureFormat::Palette32Bit:
            {
                dxt->Format = TextureFormat::Color16Bit565;
                break;
            }
            case TextureFormat::OpacityMap8Bit:
            {
                dxt->Format = State.DX.Surfaces.DrawSurfaceFormat;
                break;
            }
            case TextureFormat::Color32Bit:
            {
                dxt->Format = (dxt->Options & TEXTURE_OPTION_IGNORE_ALPHA)
                    ? TextureFormat::Color16Bit565
                    : State.DX.Surfaces.DrawSurfaceFormat;
                break;
            }
            default: { return 0; }
            }

            dxt->ActualWidth = (s16)texture->Width;
            dxt->ActualHeight = (s16)texture->Height;
            dxt->ActualMipLevelCount = texture->MipLevelCount + 1;

            auto mlc = 0;
            auto size = max(dxt->ActualHeight, dxt->ActualWidth);

            switch (size)
            {
            case 1: { mlc = 1; break; }
            case 2: { mlc = 2; break; }
            case 4: { mlc = 3; break; }
            case 8: { mlc = 4; break; }
            case 16: { mlc = 5; break; }
            case 32: { mlc = 6; break; }
            case 64: { mlc = 7; break; }
            case 128: { mlc = 8; break; }
            case 256: { mlc = 9; break; }
            case 512: { mlc = 10; break; }
            case 1024: { mlc = 11; break; }
            case 2048: { mlc = 12; break; }
            case 4096: { mlc = 13; break; }
            default:
            {
                Exit("Unknown texture size: %d x %d.", dxt->ActualWidth, dxt->ActualHeight);
                return 0;
            }
            }

            if (dxt->ActualMipLevelCount < 1 || mlc < dxt->ActualMipLevelCount)
            {
                Exit("Invalid mip level count: %d.", dxt->ActualMipLevelCount);
                return 0;
            }

            auto format = D3DFORMAT::D3DFMT_UNKNOWN;

            switch (dxt->Format)
            {
            case TextureFormat::Color32Bit: { format = D3DFORMAT::D3DFMT_A8R8G8B8; break; }
            case TextureFormat::Color16Bit565: { format = D3DFORMAT::D3DFMT_R5G6B5; break; }
            case TextureFormat::Color16Bit4444: { format = D3DFORMAT::D3DFMT_A4R4G4B4; break; }
            default: { return 0; }
            }

            auto result = State.DX.DirectXDevice->CreateTexture(dxt->ActualWidth, dxt->ActualHeight,
                dxt->ActualMipLevelCount, 0, format, D3DPOOL::D3DPOOL_MANAGED, &dxt->Texture);

            if (result != 0)
            {
                FreeTexture(indx);
                return 0;
            }

            return indx;
        }

        extern "C" BOOL __cdecl GetTextureInfo(const u32)
        {
            return FALSE;
        }

        extern "C" BOOL __cdecl SelectCubeTexture(const u32 cindx, const u32 tindx)
        {
            State.DX.Textures.Selected.Textures[tindx] = State.DX.Textures.Cube.Textures[cindx];

            return TRUE;
        }

        extern "C" BOOL __cdecl FreeTexture(const u32 indx)
        {
            if (MAX_TEXTURE_COUNT <= indx) { return FALSE; }

            auto texture = &State.DX.Textures.Textures[indx];

            if (texture->Texture != NULL)
            {
                texture->Texture->Release();
                texture->Texture = NULL;
            }

            ZeroMemory(texture, sizeof(struct Texture));

            return TRUE;
        }

        extern "C" BOOL __cdecl ResetTextureCache(void)
        {
            for (auto x = 0; x < MAX_TEXTURE_COUNT; x++)
            {
                FreeTexture(x);
            }

            return TRUE;
        }

        extern "C" BOOL __cdecl SelectTexture(const u32 tindx, const u32 sindx)
        {
            if (MAX_TEXTURE_COUNT <= tindx) { return FALSE; }

            State.DX.Textures.Selected.Textures[sindx] = State.DX.Textures.Textures[tindx].Texture;

            return TRUE;
        }

        extern "C" BOOL __cdecl SetTextureClamp(const TextureClamp u, const TextureClamp v, const u32 indx)
        {
            if (State.DX.Textures.Selected.ClampU[indx] != u || State.DX.Textures.Selected.ClampV[indx] != v)
            {
                State.DX.Textures.Selected.ClampU[indx] = u;
                State.DX.Textures.Selected.ClampV[indx] = v;

                State.DX.Textures.ClampOperation = TextureBlendOperation::Negative;
            }

            return TRUE;
        }

        void TextureConvert(const struct DX::DXTexture* dxt, const struct Texture* texture)
        {
            if (texture->Format == TextureFormat::Palette24Bit
                || texture->Format == TextureFormat::Palette32Bit
                || texture->Format == TextureFormat::OpacityMap8Bit)
            {
                for (auto x = 0; x < MAX_PALETTE_BUFFER_SIZE; x++)
                {
                    auto r = (u32)texture->Colors[x * 3 + 0];
                    auto g = (u32)texture->Colors[x * 3 + 1];
                    auto b = (u32)texture->Colors[x * 3 + 2];

                    State.DX.Textures.Buffers.Colors[x] = (r << 16) | (g << 8) | b;
                }

                if (texture->Format == TextureFormat::Palette32Bit)
                {
                    for (auto x = 0; x < MAX_PALETTE_BUFFER_SIZE; x++)
                    {
                        auto alpha = (u32)texture->Colors[MAX_PALETTE_BUFFER_SIZE * 3 + 4 + x];

                        State.DX.Textures.Buffers.Colors[x] |= alpha << 24;
                    }
                }
                else if (texture->Alphas == NULL)
                {
                    for (auto x = 0; x < MAX_PALETTE_BUFFER_SIZE; x++)
                    {
                        State.DX.Textures.Buffers.Colors[x] |= 0xff000000;
                    }
                }
            }

            auto offset = 0;
            auto width = dxt->ActualWidth;
            auto height = dxt->ActualHeight;

            for (auto x = 0; x < dxt->ActualMipLevelCount; x++)
            {
                auto size = width * height;

                switch (texture->Format)
                {
                case TextureFormat::Palette24Bit:
                case TextureFormat::Palette32Bit:
                {
                    for (auto xx = 0; xx < size; xx++)
                    {
                        auto indx = texture->Indexes[xx];
                        auto value = State.DX.Textures.Buffers.Colors[indx];

                        State.DX.Textures.Buffers.Pixels[offset + xx] = value;
                    }

                    break;
                }
                case TextureFormat::OpacityMap8Bit:
                {
                    for (auto xx = 0; xx < size; xx++)
                    {
                        auto alpha = texture->Alphas[xx];

                        auto indx = texture->Indexes[xx];
                        auto value = State.DX.Textures.Buffers.Colors[indx];

                        State.DX.Textures.Buffers.Pixels[offset + xx] = value | (alpha << 24);
                    }

                    break;
                }
                case TextureFormat::Color32Bit:
                {
                    for (auto xx = 0; xx < size; xx++)
                    {
                        State.DX.Textures.Buffers.Pixels[offset + xx] = texture->ARGB[xx];
                    }

                    break;
                }
                default: { return; }
                }

                offset += size;
                height /= 2;
                width /= 2;
            }
        }

        void TextureCopy32Bits(const u32* colors, void* bits, const s32 pitch, const s32 width, const s32 height)
        {
            auto buffer = (u32*)bits;
            auto stride = pitch / sizeof(u32);

            for (auto x = 0; x < height; x++)
            {
                for (auto xx = 0; xx < width; xx++)
                {
                    auto color = colors[height * x + xx];

                    buffer[stride * x + xx] = color;
                }
            }
        }

        void TextureCopy16Bits565(const u32* colors, void* bits, const s32 pitch, const s32 width, const s32 height)
        {
            auto buffer = (u16*)bits;
            auto stride = pitch / sizeof(u16);

            for (auto x = 0; x < height; x++)
            {
                for (auto xx = 0; xx < width; xx++)
                {
                    auto color = colors[height * x + xx];

                    u16 r = (color >> 19) << 11;
                    u16 g = ((color >> 10) & 0x3f) << 5;
                    u16 b = (color >> 3) & 0x1f;

                    buffer[stride * x + xx] = r | g | b;
                }
            }
        }

        void TextureCopy16Bits4444(const u32* colors, void* bits, const s32 pitch, const s32 width, const s32 height)
        {
            auto buffer = (u16*)bits;
            auto stride = pitch / sizeof(u16);

            for (auto x = 0; x < height; x++)
            {
                for (auto xx = 0; xx < width; xx++)
                {
                    auto color = colors[height * x + xx];

                    u16 r = (color >> 12) << 12;
                    u16 g = (color >> 20 & 0xf) << 8;
                    u16 b = (color >> 12 & 0xf) << 4;
                    u16 a = (color >> 4) & 0xf;

                    buffer[stride * x + xx] = r | g | b | a;
                }
            }
        }

        extern "C" BOOL __cdecl UploadTexture(const u32 indx, const struct Texture* texture)
        {
            if (MAX_TEXTURE_COUNT <= indx) { return FALSE; }

            auto dxt = &State.DX.Textures.Textures[indx];

            auto offset = 0;
            auto width = dxt->ActualWidth;
            auto height = dxt->ActualHeight;

            TextureConvert(dxt, texture);

            auto surface = dxt->Format == TextureFormat::Color16Bit565
                ? State.DX.Surfaces.Source565Surface
                : State.DX.Surfaces.AlphaSourceSurface;

            for (auto x = 0; x < dxt->ActualMipLevelCount; x++)
            {
                D3DLOCKED_RECT lr;
                RECT rect =
                {
                    .left = 0,
                    .top = 0,
                    .right = width,
                    .bottom = height
                };

                DX::DXC(surface->LockRect(&lr, &rect, D3DLOCK_NOSYSLOCK), "Unable to lock source texture.");

                auto pixels = &State.DX.Textures.Buffers.Pixels[offset];

                switch (dxt->Format)
                {
                case TextureFormat::Color32Bit:
                {
                    TextureCopy32Bits(pixels, lr.pBits, lr.Pitch, width, height);
                    break;
                }
                case TextureFormat::Color16Bit565:
                {
                    TextureCopy16Bits565(pixels, lr.pBits, lr.Pitch, width, height);
                    break;
                }
                case TextureFormat::Color16Bit4444:
                {
                    TextureCopy16Bits4444(pixels, lr.pBits, lr.Pitch, width, height);
                    break;
                }
                }

                DX::DXC(surface->UnlockRect(), "Unable to unlock texture level.");

                IDirect3DSurface8* sl;
                DX::DXC(dxt->Texture->GetSurfaceLevel(x, &sl), "Unable to get surface level on texture.");

                DX::DXC(State.DX.DirectXDevice->CopyRects(surface, &rect, 1, sl, NULL), "Unable to copy rectangles.");

                sl->Release();

                offset += width * height;

                width /= 2;
                height /= 2;
            }

            return TRUE;
        }

        extern "C" BOOL __cdecl CopyBackBufferToRenderTexture(void)
        {
            return FALSE;
        }
    }
}