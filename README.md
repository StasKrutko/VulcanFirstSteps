# VulcanFirstSteps
Working with vulkan from this tutorial:
https://vulkan-tutorial.com/#page_Tutorial_structure

another tutorial:
https://www.youtube.com/watch?v=IptB-XlIO1Y&list=PLxNGvnKpZBcVyjXg7Ko9RcAy9Nt-SmS1J&index=6

Known issues:
1. It seems validation layers does not correctly working on Ubuntu (during chapter "validation layers" 
   there is no message output for any debug information).
   
   Solution: LunarG Vulkan SDK has a sources for one test binary: vkvia. Running it will generate html with all
   neccessary info about Vulkan SDK (what is missing, what is present etc.). In my case, it has indicated that I
   was missing some neccessary enviroment variables.

   To make validation layers work on Ubuntu there are 2 ways:
   - there is a script: setup_env.sh. Adding it's content to .bachrc will enable vaildation layers, IF target
   binary is runned from Terminal.
   - Add LD_LIBRARY_PATH (${VULKAN_SDK}/x86-64/lib) and VK_LAYER_PATH(${VULKAN_SDK}/x86-64/etc/explicit_layer.d)
   to the IDE runtime variables. This will cause validation layers to work in IDE.
   - Add all of the paths to the /etc/enviroment, and relogin. This will enable them system-wide.
   
2. On Ubuntu, during chapter "surface KHR", there is a crash, during call vkGetPhysicalDeviceSurfaceSupportKHR.
