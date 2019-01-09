#pragma once

#include <vulkan/vulkan.h>

#include <optional>
#include <vector>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

using namespace std;

constexpr auto WINDOW_WIDTH = 800;
constexpr auto WINDOW_HEIGHT = 600;

const vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_LUNARG_standard_validation"
};

const vector<const char*> DEVICE_EXTENCIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
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
        optional<uint32_t> m_graphics_family;
        optional<uint32_t> m_present_family;

        bool is_index_complete()
        {
            return m_graphics_family.has_value() && m_present_family.has_value();
        }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR m_capabilities;
        vector<VkSurfaceFormatKHR> m_formats;
        vector<VkPresentModeKHR> m_present_modes;
    };

    void init_window();
    void init_vulkan();
    void init_setup_callback();
    void create_VK_instance();
    void destroy_debug_utils_messenger_EXT(VkInstance instance,
                                           VkDebugUtilsMessengerEXT callback,
                                           const VkAllocationCallbacks* pAllocator);
    void create_KHR_surface();
    void pick_graphic_card();
    void create_logical_device();
    void create_swap_chain();
    void create_image_views();
    void execute_main_loop();
    void cleanup();

    bool check_validation_layers_support();
    bool check_device_suitability(VkPhysicalDevice device);
    bool check_device_extensions_support(VkPhysicalDevice device);

    QueueFamilyIndex find_queue_families(VkPhysicalDevice device);
    SwapChainSupportDetails query_swapchain_support(VkPhysicalDevice device);
    VkSurfaceFormatKHR choose_swap_surface_format(const vector<VkSurfaceFormatKHR>& available_formats);
    VkPresentModeKHR   choose_swapchain_present_mode(const vector<VkPresentModeKHR>& available_presend_modes);
    VkExtent2D         choose_swapchain_extent(const VkSurfaceCapabilitiesKHR& capabilities);

    vector<const char*> get_required_extensions();
    VkResult create_debug_utils_messenger_EXT(VkInstance instance,
                                              const VkDebugUtilsMessengerCreateInfoEXT* debug_info,
                                              const VkAllocationCallbacks* allocator,
                                              VkDebugUtilsMessengerEXT* callback_object);

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
    VkQueue m_graphical_queue;
    VkQueue m_present_queue;

    VkSurfaceKHR m_surface;
    VkSwapchainKHR m_swapchain;
    vector<VkImage> m_sch_images;
    VkFormat m_sch_image_format;
    VkExtent2D m_sch_extent;

    vector<VkImageView> m_sch_image_views;
};

int call_HelloTriangleApplication();

