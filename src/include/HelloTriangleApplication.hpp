#pragma once

#include <vulkan/vulkan.h>

#include <optional>
#include <vector>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

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

    struct QueueFamilyIndex
    {
        std::optional<uint32_t> m_graphics_family;
        std::optional<uint32_t> m_present_family;

        bool is_index_complete()
        {
            return m_graphics_family.has_value() && m_present_family.has_value();
        }
    };

    void init_window();
    void init_vulkan();
    void init_setup_callback();
    void pick_graphic_card();
    QueueFamilyIndex find_queue_families(VkPhysicalDevice device);
    void create_logical_device();
    void create_VK_instance();
    void create_KHR_surface();
    void execute_main_loop();
    void cleanup();


    bool check_validation_layers_support();
    bool check_device_suitability(VkPhysicalDevice device);
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
    VkQueue m_device_queue;
    VkQueue m_present_queue;

    VkSurfaceKHR m_surface;
};

int call_HelloTriangleApplication();

