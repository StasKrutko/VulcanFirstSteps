#pragma once

#include "HelloTriangleApplication.hpp"
//PC at work has only integrated GPU inside Intel i7
#define GPU_NAME "Intel(R) HD Graphics"

void HelloTriangleApplication::create_KHR_surface()
{
    /**
      * Commented code below leads to crash during vkGetPhysicalDeviceSurfaceSupportKHR
      * TODO: investigate, why. Probably, search for glfwCreateWindowSurface realization.
      * Found platform specific realization in x11_window.c. To be investigated later.
      *
      * VkBool32 vkGetPhysicalDeviceXcbPresentationSupportKHR(
            VkPhysicalDevice                            physicalDevice,
            uint32_t                                    queueFamilyIndex,
            xcb_connection_t*                           connection,
            xcb_visualid_t                              visual_id);
      **/

//    VkXcbSurfaceCreateInfoKHR create_info = { };
//    create_info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
//    create_info.flags = 0;

//    if ( vkCreateXcbSurfaceKHR(m_instance, &create_info, nullptr, &m_surface) != VK_SUCCESS)
//    {
//        throw std::runtime_error("Failed to create KHR Surface!");
//    }
    if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create KHR Surface!");
    }
}
