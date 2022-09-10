#pragma once

#include "DXState.h"
#include "Module.h"
#include "Settings.h"
#include "WindowState.h"

struct State
{
    struct InitializeModuleArguments InitializeArguments;

    struct DXState DX;

    struct Settings Settings;

    struct WindowState Window;
};

extern struct State State;