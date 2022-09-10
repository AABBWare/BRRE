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

#include "GL.h"
#include "Graphics/Color.h"
#include "Graphics/Matrix.h"
#include "Graphics/Vertex.h"
#include "Module.h"
#include "State.h"

extern "C" u32 __cdecl PolyListBone(const void*, const u32, const u16*, const u32, const u32, const u32)
{
    return FALSE;
}

extern "C" u32 __cdecl PolyListD3DL(const struct D3DTLVertex* vertexes, const u32 vertexCount, const u16 * indexes, const u32 indexCount, const u32 mode)
{
    if (MAX_VERTEX_COUNT < vertexCount) { return FALSE; }

    GLSetMode(mode);

    glDisable(GL_LIGHTING);

    GLF.glClientActiveTextureARB(GL_TEXTURE0_ARB);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glDisableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(struct D3DTLVertex), &vertexes[0].XYZW);
    glColorPointer(4, GL_FLOAT, sizeof(struct BGRA), &State.GL.Vertexes.Colors);
    glTexCoordPointer(2, GL_FLOAT, sizeof(struct D3DTLVertex), &vertexes[0].UV);

    for (auto x = 0; x < vertexCount; x++)
    {
        auto vertex = &vertexes[x];
        auto color = &State.GL.Vertexes.Colors[x];

        color->R = 0.003921569f * vertex->Diffuse.R;
        color->G = 0.003921569f * vertex->Diffuse.G;
        color->B = 0.003921569f * vertex->Diffuse.B;
        color->A = 0.003921569f * vertex->Diffuse.A;
    }

    glDrawElements(GL_TRIANGLES, indexCount * 3, GL_UNSIGNED_SHORT, indexes);

    return TRUE;
}

extern "C" u32 __cdecl PolyListD3DTL(const struct D3DTLVertex* vertexes, const u32 vertexCount, const u16 * indexes, const u32 indexCount, const u32 mode)
{
    if (MAX_VERTEX_COUNT < vertexCount) { return FALSE; }

    GLSetMode(mode & 0xffffff37);

    glDisable(GL_LIGHTING);

    GLF.glClientActiveTextureARB(GL_TEXTURE1_ARB);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    GLF.glClientActiveTextureARB(GL_TEXTURE0_ARB);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glDisableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(struct D3DTLVertex), &vertexes[0].XYZW);
    glColorPointer(4, GL_FLOAT, sizeof(struct BGRA), &State.GL.Vertexes.Colors);
    glTexCoordPointer(2, GL_FLOAT, sizeof(struct D3DTLVertex), &vertexes[0].UV);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    auto height = State.Window.Height;
    auto width = State.Window.Width;
    auto clippingNear = State.GL.Mode.Clipping.Near;
    auto clippingFar = State.GL.Mode.Clipping.Far;

    struct Matrix4x4 matrix =
    {
         2.0f / width, 0.0f,           0.0f,                                                            0.0f,
         0.0f,         -2.0f / height, 0.0f,                                                            0.0f,
         0.0f,         0.0f,           2.0f / (clippingFar - clippingNear),                             0.0f,
         -1.0f,        1.0f,           -(clippingFar + clippingNear) / (clippingFar - clippingNear),    1.0f
    };

    glLoadMatrixf((float*)&matrix);

    for (auto x = 0; x < vertexCount; x++)
    {
        auto vertex = &vertexes[x];
        auto color = &State.GL.Vertexes.Colors[x];

        color->R = 0.003921569f * vertex->Diffuse.R;
        color->B = 0.003921569f * vertex->Diffuse.B;
        color->G = 0.003921569f * vertex->Diffuse.G;
        color->A = 0.003921569f * vertex->Diffuse.A;
    }

    glDrawElements(GL_TRIANGLES, indexCount * 3, GL_UNSIGNED_SHORT, indexes);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    return TRUE;
}

extern "C" u32 __cdecl PolyListBasis(const struct SVertexBasis* vertexes, const u32 vertexCount, const u16* indexes, const u32 indexCount, const u32 mode)
{
    if (MAX_VERTEX_COUNT < vertexCount) { return FALSE; }

    GLSetMode(mode);

    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(struct SVertexBasis), &vertexes[0].XYZ);
    glTexCoordPointer(2, GL_FLOAT, sizeof(struct SVertexBasis), &vertexes[0].UV);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    if (State.GL.Textures.MultiBlendOperation == TextureBlendOperation::None)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, sizeof(struct SVertexBasis), &vertexes[0].Normal);
    }
    else
    {
        GLF.glClientActiveTextureARB(GL_TEXTURE1_ARB);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, sizeof(struct SVertexBasis), &vertexes[0].UV1);

        GLF.glActiveTextureARB(GL_TEXTURE1_ARB);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        GLF.glActiveTextureARB(GL_TEXTURE0_ARB);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glDisableClientState(GL_NORMAL_ARRAY);
    }

    if (State.GL.Light.IsEnabled)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, sizeof(struct SVertexBasis), &vertexes[0].Normal);
        glEnable(GL_LIGHTING);
    }
    else
    {
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisable(GL_LIGHTING);
    }

    glColor3f(1.0f, 1.0f, 1.0f);

    glDrawElements(GL_TRIANGLES, indexCount * 3, GL_UNSIGNED_SHORT, indexes);

    if (State.GL.Textures.MultiBlendOperation != TextureBlendOperation::None)
    {
        GLF.glClientActiveTextureARB(GL_TEXTURE1_ARB);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        GLF.glClientActiveTextureARB(GL_TEXTURE0_ARB);
        GLF.glActiveTextureARB(GL_TEXTURE0_ARB);
        return TRUE;
    }

    glDisableClientState(GL_NORMAL_ARRAY);

    return TRUE;
}

extern "C" u32 __cdecl PolyList(const struct SVertex* vertexes, const u32 vertexCount, const u16 * indexes, const u32 indexCount, const u32 mode)
{
    if (MAX_VERTEX_COUNT < vertexCount) { return FALSE; }

    GLSetMode(mode);

    GLF.glClientActiveTextureARB(GL_TEXTURE0_ARB);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(struct SVertex), &vertexes[0].XYZ);
    glColorPointer(4, GL_FLOAT, sizeof(struct BGRA), &State.GL.Vertexes.Colors);
    glTexCoordPointer(2, GL_FLOAT, sizeof(struct SVertex), &vertexes[0].UV);
    glNormalPointer(GL_FLOAT, sizeof(struct SVertex), &vertexes[0].Normal);

    if (State.GL.Light.IsEnabled)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
    }

    if (State.GL.Textures.MultiBlendOperation == TextureBlendOperation::None
        || State.GL.Textures.MultiBlendOperation == TextureBlendOperation::AddSigned
        || State.GL.Textures.MultiBlendOperation == TextureBlendOperation::Subtract
        || State.GL.Textures.MultiBlendOperation == TextureBlendOperation::AddSmooth)
    {
        GLF.glActiveTextureARB(GL_TEXTURE1_ARB);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glDisable(GL_TEXTURE_2D);
        GLF.glActiveTextureARB(GL_TEXTURE0_ARB);
        glEnable(GL_FOG);
        glEnable(GL_LIGHTING);
    }
    else
    {
        GLF.glClientActiveTextureARB(GL_TEXTURE1_ARB);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, sizeof(struct SVertex), &vertexes[0].UV);
        GLF.glActiveTextureARB(GL_TEXTURE1_ARB);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glEnable(GL_TEXTURE_2D);
        GLF.glActiveTextureARB(GL_TEXTURE0_ARB);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

    f32 material[] = { 1.0f, 1.0f, 1.0f, State.GL.Light.Colors.Alpha };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, (f32*)&material);

    for (auto x = 0; x < vertexCount; x++)
    {
        auto color = &State.GL.Vertexes.Colors[x];

        color->R = 1.0f;
        color->G = 1.0f;
        color->B = 1.0f;
        color->A = 1.0f;
    }

    glColor3f(1.0f, 1.0f, 1.0f);

    glDrawElements(GL_TRIANGLES, indexCount * 3, GL_UNSIGNED_SHORT, indexes);

    return TRUE;
}

extern "C" u32 __cdecl PolyListTL(const struct TLVertex* vertexes, const u32 vertexCount, const u16* indexes, const u32 indexCount, const u32 mode)
{
    if (MAX_VERTEX_COUNT < vertexCount) { return FALSE; }

    GLSetMode(mode);

    glDisable(GL_LIGHTING);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(struct TLVertex), &vertexes[0].XYZ);
    glColorPointer(4, GL_FLOAT, sizeof(struct BGRA), &State.GL.Vertexes.Colors);
    glTexCoordPointer(2, GL_FLOAT, sizeof(struct TLVertex), &vertexes[0].UV);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    auto clippingNear = State.GL.Mode.Clipping.Near;
    auto clippingFar = State.GL.Mode.Clipping.Far;

    struct Matrix4x4 matrix =
    {
        1.0f, 0.0f, 0.0f,                                                       0.0f,
        0.0f, 1.0f, 0.0f,                                                       0.0f,
        0.0f, 0.0f, clippingFar / (clippingFar - clippingNear),                 1.0f,
        0.0f, 0.0f, -clippingNear * clippingFar / (clippingFar - clippingNear), 0.0f
    };

    glLoadMatrixf((float*)&matrix);

    for (auto x = 0; x < vertexCount; x++)
    {
        auto vertex = &vertexes[x];
        // Note: it looks like there is no need for this adjustment.
        //vertex->XYZ.X = vertex->XYZ.X - vertex->XYZ.Z / State.Window.Width;
        //vertex->XYZ.Y = vertex->XYZ.Y - vertex->XYZ.Z / State.Window.Height;

        auto color = &State.GL.Vertexes.Colors[x];

        color->R = vertex->RGBA.R;
        color->G = vertex->RGBA.G;
        color->B = vertex->RGBA.B;
        color->A = vertex->RGBA.A;
    }

    glDrawElements(GL_TRIANGLES, indexCount * 3, GL_UNSIGNED_SHORT, indexes);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    return TRUE;
}

extern "C" u32 __cdecl LineListD3DTL(const struct D3DTLVertex*, const u32, const u32)
{
    return FALSE;
}

extern "C" u32 __cdecl DotListD3DTL(const struct D3DTLVertex*, const u32, const u32)
{
    return FALSE;
}