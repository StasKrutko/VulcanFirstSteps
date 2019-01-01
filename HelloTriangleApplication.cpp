#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "HelloTriangleApplication.h"
#include <vector>

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
    vkDestroyInstance(m_instance, nullptr);

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void HelloTriangleApplication::create_VK_instance()
{
    if (ENABLE_VALIDATION_LAYERS && !check_validation_layers_support())
    {
        throw std::runtime_error("validation layers requested, but not available!");
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

    uint32_t glfw_extensions_count;
    const char** glfw_extensions;

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
        throw std::runtime_error("Failed to create Instance! Stupid...\n");
}

bool HelloTriangleApplication::check_validation_layers_support()
{
    uint32_t layers_count;
    vkEnumerateInstanceLayerProperties(&layers_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layers_count);

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

std::vector<const char*> HelloTriangleApplication::get_required_extensions() {
    uint32_t glfw_extensions_count = 0;
    const char** glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extensions_count);

    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extensions_count);

    if (ENABLE_VALIDATION_LAYERS) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    if (!compare_extensions(glfw_extensions, glfw_extensions_count))
    {
        std::cout << "Oops! Something wrong with extensions comparation" << std::endl;
    }

    return extensions;
}

bool HelloTriangleApplication::compare_extensions(const char** glfw_extensions, uint32_t glfw_extensions_count)
{
    //--------------checking glfw extensions with vk provided extensions
    uint32_t vk_extensions_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &vk_extensions_count, nullptr);
    std::vector<VkExtensionProperties> vk_extensions(vk_extensions_count);
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
    return result;
}

int call_HelloTriangleApplication()
{
    HelloTriangleApplication app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
