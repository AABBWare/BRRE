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

#include "GL.h"
#include "GLTexture.h"
#include "Graphics/Color.h"
#include "Graphics/Graphics.h"
#include "Mathematics/Matrix.h"
#include "Graphics/Texture.h"
#include "Graphics/Vector.h"

#define MAX_BIND_TEXTURE_COUUNT 2
#define MAX_PALETTE_BUFFER_SIZE 256
#define MAX_PIXEL_BUFFER_SIZE (1024 * 1024 + 512 * 512 + 256 * 256 + 128 * 128 + 64 * 64 + 32 * 32 + 16 * 16 + 8 * 8 + 4 * 4 + 2 * 2 + 1 * 1)
#define MAX_RENDER_TEXTURE_COUNT 8
#define MAX_SELECTED_TEXTURE_COUNT 8
#define MAX_TEXTURE_COUNT 5000
#define MAX_VERTEX_COUNT 5000

namespace Renderer
{
    namespace External
    {
        namespace GL
        {
            struct GLLight
            {
                struct Renderer::Graphics::Vector4 Position;
                struct Renderer::Graphics::RGBA Color;
            };

            struct GLState
            {
                struct
                {
                    Renderer::Graphics::CullMode Cull;

                    struct
                    {
                        f32 Near;
                        f32 Far;
                    } Clipping;

                    s32 SourceBlend = GL_SRC_ALPHA;
                    s32 DestinationBlend = GL_ONE_MINUS_SRC_ALPHA;
                } Mode;

                struct
                {
                    Renderer::Graphics::TextureBlendOperation MultiBlendOperation;

                    struct
                    {
                        u32 Textures[MAX_BIND_TEXTURE_COUUNT];
                    } Bind;

                    struct
                    {
                        Renderer::Graphics::TextureClamp S = Renderer::Graphics::TextureClamp::Wrap;
                        Renderer::Graphics::TextureClamp T = Renderer::Graphics::TextureClamp::Wrap;
                    } Clamp;

                    struct
                    {
                        u32 Textures[MAX_SELECTED_TEXTURE_COUNT];
                    } Selected;

                    struct
                    {
                        BOOL IsEnabled = FALSE;
                        u32 Index;
                        f32 Direction;

                        struct GLTexture Textures[MAX_RENDER_TEXTURE_COUNT];
                    } Render;

                    struct
                    {
                        /// <summary>
                        /// Texture 32-bit ARGB pixel colors palette.
                        /// Used during the texture uploading, and is being overwritten each time new texture is uploaded.
                        /// </summary>
                        u32 Colors[MAX_PALETTE_BUFFER_SIZE];

                        /// <summary>
                        /// Texture 32-bit ARGB pixel colors for all mip levels.
                        /// Used during the texture uploading, and is being overwritten each time new texture is uploaded.
                        /// </summary>
                        u32 Pixels[MAX_PIXEL_BUFFER_SIZE];
                    } Buffers;

                    u32 TextureCount;
                    u32 Textures[MAX_TEXTURE_COUNT];
                } Textures;

                struct
                {
                    BOOL IsEnabled = FALSE;

                    struct
                    {
                        f32 Alpha = 1.0f;

                        f32 R[8];
                        f32 G[8];
                        f32 B[8];
                    } Colors;

                    struct GLLight Lights[MAX_LIGHT_COUNT];
                } Light;

                struct
                {
                    struct Mathematics::Matrix4x4 ModelView;
                    struct Mathematics::Matrix4x4 Projection;
                } Transform;

                struct
                {
                    struct Renderer::Graphics::BGRA Colors[MAX_VERTEX_COUNT];
                } Vertexes;
            };
        }
    }
}