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

#include "App.h"

#include <shellapi.h>

const char* WindowClassName = "CutScenePlayer";
const char* WindowTitle = "CutScene Player";

IGraphBuilder* GraphBuilder;
IMediaControl* MediaControl;
IMediaEventEx* MediaEvent;
IVideoWindow* VideoWindow;

IBaseFilter* VideoDecoder;
IBaseFilter* VideoFilter;
IBaseFilter* VideoRenderer;
IBaseFilter* VideoSplitter;

IBaseFilter* AudioDecoder;
IBaseFilter* AudioFilter;
IBaseFilter* AudioRenderer;
IBaseFilter* AudioSplitter;

BOOL IsRunning;
HWND MainWindow;

s32 App(const HINSTANCE instance)
{
    s32 count;
    auto arguments = CommandLineToArgvW(GetCommandLineW(), &count);

    if (count < 2) { return 1; }
    if (arguments == NULL) { return 1; }

    ShowCursor(0);

    if (InitializeWindow(instance) != 0) { return 1; }

    auto result = count == 2
        ? Execute(arguments[1], arguments[1])
        : Execute(arguments[1], arguments[2]);

    if (VideoWindow != NULL)
    {
        DestroyWindow(MainWindow);
    }

    UnregisterClassW((LPCWSTR)WindowClassName, instance);

    return result;
}

s32 Execute(const LPCWSTR video, const LPCWSTR audio)
{
    auto result = CoInitialize(NULL);

    if (SUCCEEDED(result))
    {
        result = InitializeClasses();

        if (SUCCEEDED(result))
        {
            result = InitializePlayer(video, audio);

            if (SUCCEEDED(result))
            {
                result = InitializeVideoWindow();

                if (SUCCEEDED(result))
                {
                    result = Play();
                }
            }

            ReleaseClasses();
        }

        CoUninitialize();
    }

    return result;
}

LRESULT WindowMessageHandler(const HWND window, const UINT message, const WPARAM wp, const LPARAM lp)
{
    if (message != WM_DESTROY)
    {
        if (message == WM_KEYDOWN)
        {
            if (wp == VK_RETURN || wp == VK_ESCAPE || wp == VK_SPACE)
            {
                if (MediaControl != NULL)
                {
                    MediaControl->Stop();
                }

                IsRunning = FALSE;

                PostMessageA(MainWindow, WM_CLOSE, 0, 0);
            }
        }

        return DefWindowProcA(window, message, wp, lp);
    }

    PostQuitMessage(0);

    return 0;
}

s32 InitializeWindow(const HINSTANCE instance)
{
    WNDCLASSA windowClass = { 0 };
    windowClass.hInstance = instance;
    windowClass.lpfnWndProc = (WNDPROC)WindowMessageHandler;
    windowClass.lpszClassName = WindowClassName;

    if (RegisterClassA(&windowClass) == 0) { return -1; }

    MainWindow = CreateWindowExA(WS_OVERLAPPED, WindowClassName, WindowTitle, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 640, 480, NULL, NULL, instance, NULL);

    return MainWindow == NULL ? -1 : 0;
}

s32 InitializeClasses(void)
{
    auto result = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&GraphBuilder);

    if (FAILED(result))
    {
        ReleaseClasses();
        return result;
    }

    result = GraphBuilder->QueryInterface(IID_IMediaControl, (void**)&MediaControl);

    if (FAILED(result))
    {
        ReleaseClasses();
        return result;
    }

    result = GraphBuilder->QueryInterface(IID_IVideoWindow, (void**)&VideoWindow);

    if (FAILED(result))
    {
        ReleaseClasses();
        return result;
    }

    result = GraphBuilder->QueryInterface(IID_IMediaEventEx, (void**)&MediaEvent);

    if (FAILED(result))
    {
        ReleaseClasses();
        return result;
    }

    return 0;
}

void ReleaseClasses(void)
{
    if (MediaEvent != NULL)
    {
        MediaEvent->Release();
        MediaEvent = NULL;
    }

    if (VideoWindow != NULL)
    {
        VideoWindow->Release();
        VideoWindow = NULL;
    }

    if (MediaControl != NULL)
    {
        MediaControl->Release();
        MediaControl = NULL;
    }

    if (GraphBuilder != NULL)
    {
        GraphBuilder->Release();
        GraphBuilder = NULL;
    }
}

s32 InitializeVideoWindow(void)
{
    if (VideoWindow == NULL) { return 1; }

    VideoWindow->put_Owner((OAHWND)MainWindow);

    auto mode = 0L;
    auto result = VideoWindow->get_FullScreenMode(&mode);

    if (mode == OAFALSE)
    {
        result = VideoWindow->put_MessageDrain((OAHWND)MainWindow);

        if (FAILED(result)) { return -1; }

        result = VideoWindow->put_FullScreenMode(OATRUE);

        if (FAILED(result)) { return -1; }
    }

    ShowWindow(MainWindow, SW_SHOWMAXIMIZED);

    auto width = GetSystemMetrics(SM_CXSCREEN);
    auto height = GetSystemMetrics(SM_CYSCREEN);

    VideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
    VideoWindow->put_Left(0);
    VideoWindow->put_Top(0);
    VideoWindow->put_Width(width);
    VideoWindow->put_Height(height);

    return 0;
}

s32 InitializePlayerVideo(const LPCWSTR file)
{
    {
        auto result = GraphBuilder->AddSourceFilter(file, L"MPEG-I Stream Video Filter", &VideoFilter);

        if (FAILED(result)) { return result; }

        result = CoCreateInstance(CLSID_MPEG1Splitter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&VideoSplitter);

        if (FAILED(result)) { return result; }

        result = GraphBuilder->AddFilter(VideoSplitter, L"MPEG-I Video Splitter");

        if (FAILED(result)) { return result; }

        result = CoCreateInstance(CLSID_CMpegVideoCodec, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&VideoDecoder);

        if (FAILED(result)) { return result; }

        result = GraphBuilder->AddFilter(VideoDecoder, L"MPEG-I Video Decoder");

        if (FAILED(result)) { return result; }

        result = CoCreateInstance(CLSID_VideoRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&VideoRenderer);

        if (FAILED(result)) { return result; }

        result = GraphBuilder->AddFilter(VideoRenderer, L"MPEG-I Video Renderer");

        if (FAILED(result)) { return result; }
    }

    {
        {
            IPin* videoFilterOutPin = NULL;
            auto result = VideoFilter->FindPin(L"Output", &videoFilterOutPin);

            if (FAILED(result)) { return result; }

            IPin* videoSplitterInPin = NULL;
            result = VideoSplitter->FindPin(L"Input", &videoSplitterInPin);

            if (FAILED(result)) { return result; }

            result = GraphBuilder->Connect(videoFilterOutPin, videoSplitterInPin);

            if (FAILED(result)) { return result; }
        }

        {
            IPin* videoSplitterOutPin = NULL;
            auto result = VideoSplitter->FindPin(L"Video", &videoSplitterOutPin);

            if (FAILED(result)) { return result; }

            IPin* videoDecoderInPin = NULL;
            result = VideoDecoder->FindPin(L"In", &videoDecoderInPin);

            if (FAILED(result)) { return result; }

            result = GraphBuilder->Connect(videoSplitterOutPin, videoDecoderInPin);

            if (FAILED(result)) { return result; }
        }

        {
            IPin* videoDecoderOutPin = NULL;
            auto result = VideoDecoder->FindPin(L"Out", &videoDecoderOutPin);

            if (FAILED(result)) { return result; }

            IPin* videoRendererInPin = NULL;
            result = VideoRenderer->FindPin(L"In", &videoRendererInPin);

            if (FAILED(result)) { return result; }

            result = GraphBuilder->Connect(videoDecoderOutPin, videoRendererInPin);

            if (FAILED(result)) { return result; }
        }
    }

    return 0;
}

s32 InitializePlayerAudio(const LPCWSTR file)
{
    {
        auto result = GraphBuilder->AddSourceFilter(file, L"MPEG-I Stream Audio Filter", &AudioFilter);

        if (FAILED(result)) { return result; }

        result = CoCreateInstance(CLSID_MPEG1Splitter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&AudioSplitter);

        if (FAILED(result)) { return result; }

        result = GraphBuilder->AddFilter(AudioSplitter, L"MPEG-I Audio Splitter");

        if (FAILED(result)) { return result; }

        result = CoCreateInstance(CLSID_CMpegAudioCodec, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&AudioDecoder);

        if (FAILED(result)) { return result; }

        result = GraphBuilder->AddFilter(AudioDecoder, L"MPEG-I Audio Decoder");

        if (FAILED(result)) { return result; }

        result = CoCreateInstance(CLSID_DSoundRender, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&AudioRenderer);

        if (FAILED(result)) { return result; }

        result = GraphBuilder->AddFilter(AudioRenderer, L"MPEG-I Audio Renderer");

        if (FAILED(result)) { return result; }
    }

    {
        {
            IPin* AudioFilterOutPin = NULL;
            auto result = AudioFilter->FindPin(L"Output", &AudioFilterOutPin);

            if (FAILED(result)) { return result; }

            IPin* AudioSplitterInPin = NULL;
            result = AudioSplitter->FindPin(L"Input", &AudioSplitterInPin);

            if (FAILED(result)) { return result; }

            result = GraphBuilder->Connect(AudioFilterOutPin, AudioSplitterInPin);

            if (FAILED(result)) { return result; }
        }

        {
            IPin* AudioSplitterOutPin = NULL;
            auto result = AudioSplitter->FindPin(L"Audio", &AudioSplitterOutPin);

            if (FAILED(result)) { return result; }

            IPin* AudioDecoderInPin = NULL;
            result = AudioDecoder->FindPin(L"In", &AudioDecoderInPin);

            if (FAILED(result)) { return result; }

            result = GraphBuilder->Connect(AudioSplitterOutPin, AudioDecoderInPin);

            if (FAILED(result)) { return result; }
        }

        {
            IPin* AudioDecoderOutPin = NULL;
            auto result = AudioDecoder->FindPin(L"Out", &AudioDecoderOutPin);

            if (FAILED(result)) { return result; }

            IPin* AudioRendererInPin = NULL;
            result = AudioRenderer->FindPin(L"Audio Input pin (rendered)", &AudioRendererInPin);

            if (FAILED(result)) { return result; }

            result = GraphBuilder->Connect(AudioDecoderOutPin, AudioRendererInPin);

            if (FAILED(result)) { return result; }
        }
    }

    return 0;
}

s32 InitializePlayer(const LPCWSTR video, const LPCWSTR audio)
{
    auto result = InitializePlayerVideo(video);

    if (FAILED(result)) { return result; }

    // There is a need to support video-only files,
    // therefore, no error checks performed on audio initialization.

    InitializePlayerAudio(audio);

    return 0;
}

s32 Play(void)
{
    IsRunning = TRUE;

    auto result = MediaControl->Pause();

    if (FAILED(result)) { return result; }

    result = MediaControl->Run();

    if (FAILED(result)) { return result; }

    long eventCode = 0;
    LONG_PTR a = NULL;
    LONG_PTR b = NULL;

    do
    {
        MediaEvent->GetEvent(&eventCode, &a, &b, 0);
        MediaEvent->FreeEventParams(eventCode, a, b);

        if (eventCode == 1)
        {
            IsRunning = FALSE;
            break;
        }

        Sleep(100);

        HandleWindowMessages();

        if (!IsRunning) { break; }
    } while (TRUE);

    return 0;
}

void HandleWindowMessages(void)
{
    MSG msg = { 0 };
    auto exists = PeekMessageA(&msg, MainWindow, 0, 0, PM_REMOVE);

    while (exists)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
        exists = PeekMessageA(&msg, MainWindow, 0, 0, PM_REMOVE);
    }
}