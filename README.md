# VulcanFirstSteps
Working with vulkan from this tutorial:
https://vulkan-tutorial.com/#page_Tutorial_structure

another tutorial:
https://www.youtube.com/watch?v=IptB-XlIO1Y&list=PLxNGvnKpZBcVyjXg7Ko9RcAy9Nt-SmS1J&index=6

Known issues:
1. It seems validation layers does not correctly working on Ubuntu (during chapter "validation layers" 
   there is no message output for any debug information).
2. On Ubuntu, during chapter "surface KHR", there is a crash, during call vkGetPhysicalDeviceSurfaceSupportKHR.
