//
// Created by sfulham on 8/10/20.
//

#pragma once

#include <xcb/xcb.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>

namespace Internal {

    class LinuxVSurface {
    public:
        void Init(xcb_connection_t* connection, xcb_window_t window, VkInstance& instance, VkSurfaceKHR* surface);
    };
}