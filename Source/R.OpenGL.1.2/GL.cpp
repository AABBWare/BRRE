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
#include "Graphics/Color.h"
#include "Graphics/Enum.h"
#include "Graphics/Graphics.h"
#include "Module.h"
#include "State.h"

struct GLF GLF;

s32 GLIsExtensionSupported(const char* name)
{
    const char* const extensions = (const char*)glGetString(GL_EXTENSIONS);

    if (strstr(extensions, name) != NULL) { return TRUE; }

    if (GLF.wglGetExtensionsStringARB != NULL)
    {
        const char* const arbs = GLF.wglGetExtensionsStringARB(State.Window.DeviceContext);

        if (strstr(arbs, name) != NULL) { return TRUE; }
    }

    if (GLF.wglGetExtensionsStringEXT != NULL)
    {
        const char* const exts = GLF.wglGetExtensionsStringEXT();

        if (strstr(exts, name)) { return TRUE; }
    }

    return FALSE;
}

void GLInitializeExtensions(void)
{
    GLF.wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
    GLF.wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");

    if (GLIsExtensionSupported("GL_ARB_multitexture"))
    {
        GLF.glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
        GLF.glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
        GLF.glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTextureARB");
        
        GLF.glActiveTextureARB(GL_TEXTURE1_ARB);
        glDisable(GL_TEXTURE_2D);
        GLF.glActiveTextureARB(GL_TEXTURE0_ARB);
        GLF.glClientActiveTextureARB(GL_TEXTURE0_ARB);
    }
    else
    {
        Exit("OpenGL extension <GL_ARB_MultiTexture> not supported.");
    }

    if (GLIsExtensionSupported("WGL_ARB_render_texture"))
    {
        GLF.wglBindTexImageARB = (PFNWGLBINDTEXIMAGEARBPROC)wglGetProcAddress("wglBindTexImageARB");
        GLF.wglReleaseTexImageARB = (PFNWGLRELEASETEXIMAGEARBPROC)wglGetProcAddress("wglReleaseTexImageARB");
        GLF.wglSetPbufferAttribARB = (PFNWGLSETPBUFFERATTRIBARBPROC)wglGetProcAddress("wglSetPbufferAttribARB");
        GLF.wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
        GLF.wglCreatePbufferARB = (PFNWGLCREATEPBUFFERARBPROC)wglGetProcAddress("wglCreatePbufferARB");
        GLF.wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARBPROC)wglGetProcAddress("wglGetPbufferDCARB");
        GLF.wglQueryPbufferARB = (PFNWGLQUERYPBUFFERARBPROC)wglGetProcAddress("wglQueryPbufferARB");
        GLF.wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)wglGetProcAddress("wglReleasePbufferDCARB");
        GLF.wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARBPROC)wglGetProcAddress("wglDestroyPbufferARB");
    }
    else
    {
        Exit("OpenGL extension <WGL_ARB_Render_Texture> not supported.");
    }

    return;
}

s32 GLInitializeARBTexture(struct GLTexture* texture, const s32 width, const s32 height)
{
    s32 pixelFormats[256];
    u32 pixelFormatCount = 0;

    const s32 cpfAttributes[] = {
        WGL_DRAW_TO_PBUFFER_EXT, GL_TRUE,
        WGL_BIND_TO_TEXTURE_RGB_ARB, GL_TRUE,
        WGL_DEPTH_BITS_ARB, BITS_PER_PIXEL_24,
        WGL_DOUBLE_BUFFER_ARB, GL_FALSE,
        GL_ZERO, GL_ZERO
    };

    auto pixelFormat = GLF.wglChoosePixelFormatARB(State.Window.DeviceContext, (s32*)&cpfAttributes, NULL,
        ARRAYSIZE(pixelFormats), pixelFormats, &pixelFormatCount);
    
    if (pixelFormat == 0) { return FALSE; }
    
    const s32 cbAttributes[] = {
        WGL_TEXTURE_FORMAT_ARB, WGL_TEXTURE_RGB_ARB,
        WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB,
        WGL_MIPMAP_TEXTURE_ARB, GL_FALSE,
        WGL_PBUFFER_LARGEST_EXT, GL_FALSE,
        GL_ZERO, GL_ZERO
    };

    texture->Buffer = GLF.wglCreatePbufferARB(State.Window.DeviceContext, pixelFormats[pixelFormat],
        width, height, (s32*)&cbAttributes);

    texture->DeviceContext = GLF.wglGetPbufferDCARB(texture->Buffer);
    texture->RenderContext = wglCreateContext(texture->DeviceContext);

    wglShareLists(wglGetCurrentContext(), texture->RenderContext);

    GLF.wglQueryPbufferARB(texture->Buffer, WGL_PBUFFER_WIDTH_EXT, &texture->Width);
    GLF.wglQueryPbufferARB(texture->Buffer, WGL_PBUFFER_HEIGHT_EXT, &texture->Height);

    return TRUE;
}

void GLInitialize(void)
{
   ResetTextureCache();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glViewport(0, 0, State.Window.Width, State.Window.Height);
    glFrustum(-1.875, 1.875, -1.875, 1.875, 1.875, 6.0);
    glDepthRange(0, 1.875);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.00392157f);
    glDisable(GL_FOG);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ZERO);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DITHER);
    glFrontFace(GL_CCW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    GLInitializeExtensions();

    for (auto x = 0; x < MAX_RENDER_TEXTURE_COUNT; x++)
    {
        auto texture = &State.GL.Textures.Render.Textures[x];

        GLInitializeARBTexture(texture, 256, 256);

        glGenTextures(1, &texture->ID);
    }

    State.GL.Textures.Render.IsEnabled = FALSE;

    for (auto x = 0; x < MAX_SELECTED_TEXTURE_COUNT; x++)
    {
        State.GL.Textures.Selected.Textures[x] = 0;
    }
    
    State.GL.Textures.Render.Direction = 1.0f;
}

void GLReleaseTexture(struct GLTexture* texture)
{
    if (texture->ID)
    {
        auto hglrc = wglGetCurrentContext();

        if (hglrc == texture->RenderContext)
        {
            wglMakeCurrent(NULL, NULL);
        }

        wglDeleteContext(texture->RenderContext);

        GLF.wglReleasePbufferDCARB(texture->Buffer, texture->DeviceContext);
        GLF.wglDestroyPbufferARB(texture->Buffer);

        texture->ID = 0;
    }
}

void GLRelease(void)
{
    ResetTextureCache();

    for (auto x = 0; x < MAX_RENDER_TEXTURE_COUNT; x++)
    {
        GLReleaseTexture(&State.GL.Textures.Render.Textures[x]);
    }

    wglMakeCurrent(NULL, NULL);

    if (State.Window.RenderContext != NULL)
    {
        wglDeleteContext(State.Window.RenderContext);
        State.Window.RenderContext = NULL;
    }
    if (State.Window.DeviceContext != NULL)
    {
        ReleaseDC(State.Window.HWND, State.Window.DeviceContext);
        State.Window.DeviceContext = NULL;
    }
}

void GLSetCullMode(const CullMode mode)
{
    switch (mode)
    {
        case CullMode::CCW:
        {
            glFrontFace(GL_CCW);

            if (State.GL.Textures.Render.IsEnabled)
            {
                glCullFace(GL_BACK);
            }
            else
            {
                glCullFace(GL_FRONT);
            }

            glEnable(GL_CULL_FACE);

            return;
        }
        case CullMode::CW:
        {
            glFrontFace(GL_CW);

            if (State.GL.Textures.Render.IsEnabled)
            {
                glCullFace(GL_FRONT);
            }
            else
            {
                glCullFace(GL_BACK);
            }

            glEnable(GL_CULL_FACE);

            return;
        }
        case CullMode::None:
        {
            glDisable(GL_CULL_FACE);
            return;
        }
    }
}

void GLSetMode(const u32 mode)
{
    GLF.glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);

    if (mode & GRAPHICS_MODE_IS_ALPHA_BLEND_ENABLED)
    {
        glEnable(GL_BLEND);
        glEnable(GL_ALPHA_TEST);
    }
    else
    {
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
    }

    glBlendFunc(State.GL.Mode.SourceBlend, State.GL.Mode.DestinationBlend);

    if (mode & GRAPHICS_MODE_IS_FOG_ENABLED)
    {
        glEnable(GL_FOG);
    }
    else
    {
        glDisable(GL_FOG);
    }

    if (mode & (GRAPHICS_MODE_IS_DEPTH_BUFFER_WRITES_ENABLED | GRAPHICS_MODE_IS_DEPTH_BUFFER_ENABLED))
    {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }
    else if (mode & GRAPHICS_MODE_IS_DEPTH_BUFFER_WRITES_ENABLED)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
    }
    else if (mode & GRAPHICS_MODE_IS_ALPHA_BLEND_ENABLED)
    {
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_ALWAYS);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_ALWAYS);
    }

    if (State.GL.Light.IsEnabled)
    {
        glEnable(GL_LIGHTING);
    }
    else
    {
        glDisable(GL_LIGHTING);
    }

    GLSetCullMode(State.GL.Mode.Cull);

    if (State.GL.Textures.MultiBlendOperation == TextureBlendOperation::None
        || State.GL.Textures.MultiBlendOperation == TextureBlendOperation::AddSigned
        || State.GL.Textures.MultiBlendOperation == TextureBlendOperation::Subtract
        || State.GL.Textures.MultiBlendOperation == TextureBlendOperation::AddSmooth)
    {
        GLF.glClientActiveTextureARB(GL_TEXTURE1_ARB);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
        GLF.glClientActiveTextureARB(GL_TEXTURE0_ARB);
    }

    GLF.glActiveTextureARB(GL_TEXTURE1_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLF.glActiveTextureARB(GL_TEXTURE0_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLF.glActiveTextureARB(GL_TEXTURE1_ARB);
    glDisable(GL_TEXTURE_2D);

    GLF.glActiveTextureARB(GL_TEXTURE2_ARB);
    glDisable(GL_TEXTURE_2D);

    GLF.glActiveTextureARB(GL_TEXTURE3_ARB);
    glDisable(GL_TEXTURE_2D);

    GLF.glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);

    s32 indexes[MAX_BIND_TEXTURE_COUUNT] = { 0, -1 };
    switch (State.GL.Textures.MultiBlendOperation)
    {
        case TextureBlendOperation::Disable:
        case TextureBlendOperation::BlendTextureAlpha:
        {
            indexes[0] = 1;
            indexes[1] = 0;
            break;
        }
        case TextureBlendOperation::SelectArgument1:
        case TextureBlendOperation::BlendFactorAlpha:
        case TextureBlendOperation::BlendTextureAlphaPreMultiplied:
        case TextureBlendOperation::BlendCurrentAlpha:
        {
            indexes[0] = 1;
            indexes[1] = 4;
            break;
        }
        case TextureBlendOperation::SelectArgument2:
        {
            indexes[0] = 2;
            break;
        }
        case TextureBlendOperation::Modulate:
        {
            indexes[0] = 1;
            break;
        }
        case TextureBlendOperation::Add:
        {
            indexes[0] = 0;
            indexes[1] = 1;
            break;
        }
        case TextureBlendOperation::AddSigned2X:
        case TextureBlendOperation::PreModulate:
        {
            indexes[0] = 0;
            indexes[1] = 3;
            break;
        }
        case TextureBlendOperation::BlendDiffuseAlpha:
        {
            indexes[0] = 1;
            indexes[1] = 4;
            break;
        }
    }

    u32 textures[MAX_BIND_TEXTURE_COUUNT] = { 0, 0 };

    for (auto x = 0; x < MAX_BIND_TEXTURE_COUUNT; x++)
    {
        if (indexes[x] < 0)
        {
            textures[x] = 0;
        }
        else
        {
            textures[x] = State.GL.Textures.Selected.Textures[indexes[x]];
        }

        auto tex = textures[x];
        if (tex != State.GL.Textures.Bind.Textures[x] && (tex != 0))
        {
            GLBindTexture(x, tex);
        }

        State.GL.Textures.Bind.Textures[x] = textures[x];
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void GLBindTexture(const u32 indx, const u32 texture)
{
    switch (indx)
    {
        case 0:
        {
            GLF.glActiveTextureARB(GL_TEXTURE0_ARB);
            break;
        }
        case 1:
        {
            GLF.glActiveTextureARB(GL_TEXTURE1_ARB);
            break;
        }
        case 2:
        {
            GLF.glActiveTextureARB(GL_TEXTURE2_ARB);
            break;
        }
        case 3:
        {
            GLF.glActiveTextureARB(GL_TEXTURE3_ARB);
            break;
        }
        default: { return; }
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLF.glActiveTextureARB(GL_TEXTURE0_ARB);
}