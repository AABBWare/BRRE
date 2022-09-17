#pragma once

#include "DXState.h"
#include "DXWindowState.h"
#include "Module.h"
#include "Settings.h"

namespace Renderer
{
    namespace External
    {
        struct State
        {
            struct Renderer::External::InitializeModuleArguments InitializeArguments;

            struct DX::DXState DX;

            struct Settings::SettingsState Settings;

            struct DX::Window::DXWindowState Window;
        };

        extern struct State State;
    }
}