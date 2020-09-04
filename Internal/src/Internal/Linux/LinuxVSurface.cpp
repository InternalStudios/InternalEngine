//
// Created by sfulham on 8/10/20.
//

#include "LinuxVSurface.h"

namespace Internal
{
    void LinuxVSurface::Init(xcb_connection_t* connection, xcb_window_t window, VkInstance& instance, VkSurfaceKHR* surface)
    {
        VkXcbSurfaceCreateInfoKHR createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        createInfo.connection = connection;
        createInfo.window = window;

        if(vkCreateXcbSurfaceKHR(instance, &createInfo, nullptr, surface))
        {

        }
    }
}