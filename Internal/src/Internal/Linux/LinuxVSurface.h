//
// Created by sfulham on 8/10/20.
//

#pragma once

#include <vulkan/vulkan.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>

namespace Internal {

    class LinuxVSurface {
    public:
        void Init(::Window window, ::Display* display, VkInstance& instance, VkSurfaceKHR* surface);
    };
}