//
// Created by sfulham on 8/10/20.
//

#pragma once

#include <vulkan/vulkan.h>
#include "WindowsWindow.h"
#include <windows.h>



namespace Internal {

    class WindowsVSurface {
    public:
        void Init(HWND hwnd, HINSTANCE hinstance, VkInstance& instance, VkSurfaceKHR* surface);
    };

}
