#pragma once

#include "HelloTriangleApplication.hpp"
//my laptor has 2 GPUs. I will use the discrete one
#define GPU_NAME "GeForce"

void HelloTriangleApplication::create_KHR_surface()
{
    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = glfwGetWin32Window(m_window);
    createInfo.hinstance = GetModuleHandle(nullptr);

    if ( vkCreateWin32SurfaceKHR(m_instance, &create_info, nullptr, &m_surface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create KHR Surface!");
    }
}
