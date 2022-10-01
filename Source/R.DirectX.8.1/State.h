#pragma once

#include "DX.State.h"
#include "DX.Window.State.h"
#include "Module.h"
#include "Settings.h"

namespace Renderer::Module
{
    struct State
    {
        struct InitializeModuleArguments InitializeArguments;

        struct DX::DXState DX;

        struct Settings::SettingsState Settings;

        struct DX::DXWindowState Window;
    };

    extern struct State State;
}