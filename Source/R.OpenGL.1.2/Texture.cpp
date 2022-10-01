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

using namespace Renderer::Enums;
using namespace Renderer::Textures;

namespace Renderer::Module
{
    extern "C" u32 __cdecl AllocateTexture(const struct Texture* texture)
    {
        if (MAX_TEXTURE_COUNT <= State.GL.Textures.TextureCount) { return 0; }

        u32 id = 0;

        glGenTextures(1, &id);

        State.GL.Textures.Textures[State.GL.Textures.TextureCount] = id;
        State.GL.Textures.TextureCount++;

        return id;
    }

    extern "C" BOOL __cdecl GetTextureInfo(const u32)
    {
        return FALSE;
    }

    extern "C" BOOL __cdecl SelectCubeTexture(const u32, const u32)
    {
        return FALSE;
    }

    extern "C" BOOL __cdecl FreeTexture(const u32 id)
    {
        if (id != 0)
        {
            glDeleteTextures(1, &id);
        }

        return TRUE;
    }

    extern "C" BOOL __cdecl ResetTextureCache(void)
    {
        State.GL.Textures.TextureCount = 0;

        for (auto x = 0; x < MAX_TEXTURE_COUNT; x++)
        {
            auto id = State.GL.Textures.Textures[x];

            if (id != 0)
            {
                glDeleteTextures(1, &id);
            }
        }

        return TRUE;
    }

    extern "C" BOOL __cdecl SelectTexture(const u32 id, const u32 indx)
    {
        if (MAX_SELECTED_TEXTURE_COUNT <= indx) { return FALSE; }

        State.GL.Textures.Selected.Textures[indx] = id;

        return TRUE;
    }

    extern "C" BOOL __cdecl SetTextureClamp(const ClampMode s, const ClampMode t, const u32)
    {
        State.GL.Textures.Clamp.S = s;
        State.GL.Textures.Clamp.T = t;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s == ClampMode::Wrap ? GL_REPEAT : GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t == ClampMode::Wrap ? GL_REPEAT : GL_CLAMP);

        return TRUE;
    }

    void* TexturePixels(const struct Texture* texture, const s32 width)
    {
        auto txw = texture->Width;

        if (txw == width) { return &State.GL.Textures.Buffers.Pixels; }

        auto offset = 0;

        do
        {
            offset += txw * txw * sizeof(u32);
            txw /= 2;

            if (txw == 0)
            {
                Exit("Unable to get texture source size: %d x %d.", width, width);
            }
        } while (txw != width);

        return &State.GL.Textures.Buffers.Pixels[offset];
    }

    void TextureConvert(const struct Texture* texture)
    {
        if (texture->Colors != NULL)
        {
            for (auto x = 0; x < MAX_PALETTE_BUFFER_SIZE; x++)
            {
                auto r = (u32)texture->Colors[x * 3 + 0];
                auto g = (u32)texture->Colors[x * 3 + 1];
                auto b = (u32)texture->Colors[x * 3 + 2];

                State.GL.Textures.Buffers.Colors[x] = (r << 16) | (g << 8) | b;
            }
        }

        auto levels = texture->MipLevelCount + 1;
        auto width = texture->Width;
        auto offset = 0;

        for (auto x = 0; x < levels; x++)
        {
            auto size = width * width;
            auto pixels = &State.GL.Textures.Buffers.Pixels[offset];

            switch (texture->Format)
            {
            case TextureFormat::Palette24Bit:
            case TextureFormat::Palette32Bit:
            {
                for (auto x = 0; x < size; x++)
                {
                    auto indx = texture->Indexes[x];
                    auto value = State.GL.Textures.Buffers.Colors[indx];

                    pixels[x] = value | 0xff000000;
                }

                break;
            }
            case TextureFormat::OpacityMap8Bit:
            {
                for (auto x = 0; x < size; x++)
                {
                    auto indx = texture->Indexes[x];
                    auto alpha = texture->Alphas[x];
                    auto value = State.GL.Textures.Buffers.Colors[indx];

                    pixels[x] = value | (alpha << 24);
                }

                break;
            }
            case TextureFormat::Color32Bit:
            {
                for (auto x = 0; x < size; x++)
                {
                    pixels[x] = texture->ARGB[x];
                }

                break;
            }
            }

            offset += size;
            width /= 2;
        }
    }

    extern "C" BOOL __cdecl UploadTexture(const u32 id, const struct Texture* texture)
    {
        TextureConvert(texture);

        glBindTexture(GL_TEXTURE_2D, id);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        auto width = texture->Width;
        auto levels = texture->MipLevelCount + 1;

        for (auto x = 0; x < levels; x++)
        {
            auto pixels = TexturePixels(texture, width);
            glTexImage2D(GL_TEXTURE_2D, x, GL_RGBA8, width, width, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);
            width /= 2;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, levels);

        return TRUE;
    }

    extern "C" BOOL __cdecl CopyBackBufferToRenderTexture(void)
    {
        return FALSE;
    }
}