#include "HelloTriangle_platform.hpp"
#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <cstring>
#include <set>

HelloTriangleApplication::HelloTriangleApplication()
    : m_gpu(VK_NULL_HANDLE)
{
}

void HelloTriangleApplication::run()
{
    init_window();
	init_vulkan();
    execute_main_loop();
    cleanup();
}

void HelloTriangleApplication::init_window()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulcan", nullptr, nullptr);
}

void HelloTriangleApplication::init_vulkan()
{
    create_VK_instance();
    if (ENABLE_VALIDATION_LAYERS)
    {
        init_setup_callback();
    }
    create_KHR_surface();
    pick_graphic_card();
    create_logical_device();
}

void HelloTriangleApplication::init_setup_callback()
{
    VkDebugUtilsMessengerCreateInfoEXT debug_info = {};
    debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_info.messageSeverity =  VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                           | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
                           | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debug_info.pfnUserCallback = debug_callback;
    debug_info.pUserData = nullptr;

    if ( create_debug_utils_messenger_EXT(m_instance, &debug_info, nullptr, &m_callback) != VK_SUCCESS)
    {
        throw runtime_error("Failed to create DEBUG layer!");
    }
}

void HelloTriangleApplication::pick_graphic_card()
{
    uint32_t devices_count;
    vkEnumeratePhysicalDevices(m_instance, &devices_count, nullptr);
    if (devices_count == 0)
    {
        throw std::runtime_error("Failed to find physical devices!");
    }
    vector<VkPhysicalDevice> devices(devices_count);
    vkEnumeratePhysicalDevices(m_instance, &devices_count, devices.data());
    for (const auto& device : devices)
    {
        if ( check_device_suitability( device ) )
        {
            m_gpu = device;
            break;
        }
    }

    if (m_gpu == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Could not find suitable physical device!");
    }
}

HelloTriangleApplication::QueueFamilyIndex HelloTriangleApplication::find_queue_families(VkPhysicalDevice device)
{
    QueueFamilyIndex indices;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

    vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());


    for (int i = 0; i < queue_families.size(); ++i)
    {
        VkBool32 present_support = false;
        if (vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &present_support) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to check for surface compatability!");
        }
        if (queue_families[i].queueCount > 0 &&
            queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT &&
            present_support)
        {
            indices.m_graphics_family = i;
            indices.m_present_family = i;
        }

        if (indices.is_index_complete())
        {
            break;
        }
    }

    return indices;
}

HelloTriangleApplication::SwapChainSupportDetails HelloTriangleApplication::query_swapchain_support(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &(details.m_capabilities));

    uint32_t formats_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formats_count, nullptr);

    if (formats_count != 0)
    {
        details.m_formats.resize(formats_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formats_count, details.m_formats.data());
    }

    uint32_t present_modes_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &present_modes_count, nullptr);

    if (present_modes_count != 0)
    {
        details.m_present_modes.resize(present_modes_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &present_modes_count, details.m_present_modes.data());
    }

    return details;
}

VkSurfaceFormatKHR HelloTriangleApplication::choose_swap_surface_format(const vector<VkSurfaceFormatKHR> &available_formats)
{
    if (available_formats.size() == 1 && available_formats[0].format == VK_FORMAT_UNDEFINED)
    {
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    for (const auto& format : available_formats)
    {
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return format;
        }
    }
    return available_formats[0];
}

void HelloTriangleApplication::create_logical_device()
{
    auto family_indeces = find_queue_families(m_gpu);

    vector<VkDeviceQueueCreateInfo> queue_create_infos;
    set<uint32_t> unique_queue_families =
    {
        family_indeces.m_graphics_family.value(),
        family_indeces.m_present_family.value()
    };

    float queue_priority = 1.0f;
    for (uint32_t queue_family : unique_queue_families)
    {
        VkDeviceQueueCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        create_info.queueFamilyIndex = queue_family;
        create_info.queueCount = 1;
        create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(create_info);
    }

    VkPhysicalDeviceFeatures device_features = {};

    VkDeviceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pQueueCreateInfos = queue_create_infos.data();
    create_info.queueCreateInfoCount = 1;
    create_info.pEnabledFeatures = &device_features;
    create_info.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENCIONS.size());
    create_info.ppEnabledExtensionNames = DEVICE_EXTENCIONS.data();

    if (ENABLE_VALIDATION_LAYERS)
    {
        create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    }
    else
    {
        create_info.enabledLayerCount = 0;
    }

    if (vkCreateDevice(m_gpu, &create_info, nullptr, &m_device) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(m_device, family_indeces.m_graphics_family.value(), 0, &m_graphical_queue);
    vkGetDeviceQueue(m_device, family_indeces.m_present_family.value(), 0, &m_present_queue);
}

void HelloTriangleApplication::execute_main_loop()
{
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
    }
}

void HelloTriangleApplication::cleanup()
{
    vkDestroyDevice(m_device, nullptr);
    if (ENABLE_VALIDATION_LAYERS)
    {
        destroy_debug_utils_messenger_EXT(m_instance, m_callback, nullptr);
    }
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    vkDestroyInstance(m_instance, nullptr);

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void HelloTriangleApplication::create_VK_instance()
{
    if (ENABLE_VALIDATION_LAYERS && !check_validation_layers_support())
    {
        throw runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Hello Triangle";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    auto extensions = get_required_extensions();
    create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    create_info.ppEnabledExtensionNames = extensions.data();

    if (ENABLE_VALIDATION_LAYERS) 
    {
        create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    }
    else 
    {
        create_info.enabledLayerCount = 0;
    }

    auto result = vkCreateInstance(&create_info, nullptr, &m_instance);

    if (result != VK_SUCCESS)
        throw runtime_error("Failed to create Instance! Stupid...\n");
}

bool HelloTriangleApplication::check_validation_layers_support()
{
    uint32_t layers_count;
    vkEnumerateInstanceLayerProperties(&layers_count, nullptr);

    vector<VkLayerProperties> available_layers(layers_count);

    vkEnumerateInstanceLayerProperties(&layers_count, available_layers.data());

    for (const char* layer_name : VALIDATION_LAYERS) {
        bool layer_found = false;

        for (const auto& layer_properties : available_layers) {
            if (strcmp(layer_name, layer_properties.layerName) == 0) {
                layer_found = true;
                break;
            }
        }

        if (!layer_found) {
            return false;
        }
    }

    return true;
}

bool HelloTriangleApplication::check_device_suitability(VkPhysicalDevice device)
{
    auto indeces = find_queue_families(device);

    bool extension_supported = check_device_extensions_support(device);

    bool swap_chain_good = false;

    if (extension_supported)
    {
        auto details = query_swapchain_support(device);
        swap_chain_good = (!details.m_formats.empty() && !details.m_present_modes.empty());
    }

    return indeces.is_index_complete() && extension_supported && swap_chain_good;
}

bool HelloTriangleApplication::check_device_extensions_support(VkPhysicalDevice device)
{
    uint32_t extensions_count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensions_count, nullptr);
    vector<VkExtensionProperties> available_extensions(extensions_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensions_count, available_extensions.data());

    set<string> required_extensions(DEVICE_EXTENCIONS.begin(), DEVICE_EXTENCIONS.end());

    for (const auto& extension : available_extensions)
    {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

vector<const char*> HelloTriangleApplication::get_required_extensions() {
    uint32_t glfw_extensions_count = 0;
    const char** glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extensions_count);

    vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extensions_count);

    if (ENABLE_VALIDATION_LAYERS)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

//    cout << "GLFW required extensions:" << endl;

//    for (const auto& extension : extensions)
//    {
//        cout << "  " << extension << endl;
//    }

    if (!compare_extensions(glfw_extensions, glfw_extensions_count))
    {
        cout << "Oops! Something wrong with extensions comparation" << endl;
    }

    return extensions;
}

VkResult HelloTriangleApplication::create_debug_utils_messenger_EXT(VkInstance instance,
                                                                    const VkDebugUtilsMessengerCreateInfoEXT *debug_info,
                                                                    const VkAllocationCallbacks *allocator,
                                                                    VkDebugUtilsMessengerEXT *callback_object)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        VkResult res = func(instance, debug_info, allocator, callback_object);
        return res;
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void HelloTriangleApplication::destroy_debug_utils_messenger_EXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(instance, callback, pAllocator);
    }
}

bool HelloTriangleApplication::compare_extensions(const char** glfw_extensions, uint32_t glfw_extensions_count)
{
    //--------------checking glfw extensions with vk provided extensions
    uint32_t vk_extensions_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &vk_extensions_count, nullptr);
    vector<VkExtensionProperties> vk_extensions(vk_extensions_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &vk_extensions_count, vk_extensions.data());

    bool result = true;

    for (int i = 0; i < glfw_extensions_count; ++i)
    {
        bool extension_found = false;
        for (const auto& extension : vk_extensions)
        {
            if (strcmp(glfw_extensions[0], extension.extensionName) != 0)
            {
                extension_found = true;
                break;
            }
        }

        result = (result && extension_found);
        if (!result)
        {
            break;
        }
    }

//    cout << "VK available extensions:" << endl;
//    for (const auto& extension : vk_extensions)
//    {
//        cout << "   " << extension.extensionName << endl;
//    }

    return result;
}

VKAPI_ATTR VkBool32 VKAPI_CALL HelloTriangleApplication::debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    cerr << "validation layer: " << pCallbackData->pMessage << endl;

    return VK_FALSE;
}

int call_HelloTriangleApplication()
{
    HelloTriangleApplication app;

    try {
        app.run();
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
