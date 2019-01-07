cmake_minimum_required(VERSION 3.10)
project(VulkanTest C CXX)
include(FindPkgConfig)

set(CMAKE_CXX_STANDARD 17)
set(VULKAN_SDK_PATH /home/skrutko/Downloads/external_libraries/Vulkan/1.1.92.1)
set(SOURCES_PATH src/)
set(INCLUDES_PATH ${SOURCES_PATH}/include)

add_definitions(-DGLFW_INCLUDE_VULKAN)

if(WIN32)
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR -DGLFW_EXPOSE_NATIVE_WIN32)
    set(PLATFORM_PATH ${SOURCES_PATH}/windows)
endif(WIN32)

if(UNIX)
    add_definitions(-DVK_USE_PLATFORM_XCB_KHR -DGLFW_EXPOSE_NATIVE_X11)
    set(PLATFORM_PATH ${SOURCES_PATH}/linux)
endif(UNIX)

set(SOURCES
    ${SOURCES_PATH}/HelloTriangleApplication.cpp
    ${SOURCES_PATH}/main.cpp
    ${INCLUDES_PATH}/Getting_started.hpp
    ${INCLUDES_PATH}/HelloTriangleApplication.hpp
    ${PLATFORM_PATH}/HelloTriangle_platform.hpp
#    ${SOURCES_PATH}/TutorialExample.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${VULKAN_SDK_PATH}/x86_64/include
    ${INCLUDES_PATH}
    ${PLATFORM_PATH}
    )

find_library(VULKAN_SDK NAMES vulkan vulkan-1 HINTS ${VULKAN_SDK_PATH}/x86_64/lib)
pkg_search_module(GLFW REQUIRED glfw3)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${VULKAN_SDK}
    ${GLM_INCLUDE_DIRS}
    ${GLFW_STATIC_LIBRARIES}
    )

