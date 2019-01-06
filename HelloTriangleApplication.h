#pragma once

#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

constexpr auto WINDOW_WIDTH = 800;
constexpr auto WINDOW_HEIGHT = 600;

const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_LUNARG_standard_validation"
};

#ifdef NDEBUG
#define ENABLE_VALIDATION_LAYERS false
#else
#define ENABLE_VALIDATION_LAYERS true
#endif

class HelloTriangleApplication
{
public:
    HelloTriangleApplication();
	~HelloTriangleApplication() = default;

	void run();
private:
    void init_window();
    void init_vulkan();
    void init_setup_callback();
    void pick_graphic_card();
    void create_logical_device();
    void execute_main_loop();
    void cleanup();
    void create_VK_instance();

    bool check_validation_layers_support();
    bool check_device_suitability(VkPhysicalDevice device);
    bool check_device_queue_families(VkPhysicalDevice device);
    std::vector<const char*> get_required_extensions();

    VkResult create_debug_utils_messenger_EXT(VkInstance instance,
                                              const VkDebugUtilsMessengerCreateInfoEXT* debug_info,
                                              const VkAllocationCallbacks* allocator,
                                              VkDebugUtilsMessengerEXT* callback_object);

    void destroy_debug_utils_messenger_EXT(VkInstance instance,
                                           VkDebugUtilsMessengerEXT callback,
                                           const VkAllocationCallbacks* pAllocator);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

 //------------------test
    bool compare_extensions(const char ** glfw_extensions, uint32_t glfw_extensions_count);
//-----------------------
    VkInstance  m_instance;
    GLFWwindow* m_window;

    VkDebugUtilsMessengerEXT m_callback;
    VkPhysicalDevice m_gpu;
    VkDevice m_device;
    std::optional< uint32_t > m_queue_family_index;
    VkQueue m_device_queue;
};

int call_HelloTriangleApplication();

