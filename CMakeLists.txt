cmake_minimum_required(VERSION 3.10)
project(VulkanTest C CXX)
include(FindPkgConfig)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDES_PATH ${SOURCES_PATH}/include)

add_definitions(-DGLFW_INCLUDE_VULKAN)

#C:\external\libs\glfw-3.2.1.bin.WIN64\include - lib
#x - just Include
#C:\external\libs\VulkanSDK\1.1.92.1\Include - lib

if(WIN32)
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR -DGLFW_EXPOSE_NATIVE_WIN32)
    set(PLATFORM_PATH ${SOURCES_PATH}/windows)
    set(GLM_INCLUDE_DIR $ENV{EXTERNAL_LIBRARIES}/glm-0.9.9.3/glm)
    set(GLFW_DIR $ENV{EXTERNAL_LIBRARIES}/glfw-3.2.1.bin.WIN64/)
    find_library(VULKAN_SDK NAMES vulkan vulkan-1 HINTS $ENV{VK_SDK_PATH}/Lib)
    find_library(GLFW_LIB NAMES glfw3 HINTS ${GLFW_DIR}/lib-vc2015)
endif(WIN32)

if(UNIX)
    add_definitions(-DVK_USE_PLATFORM_XCB_KHR -DGLFW_EXPOSE_NATIVE_X11)
    set(PLATFORM_PATH ${SOURCES_PATH}/linux)
    set(GLM_INCLUDE_DIR $ENV{EXTERNAL_LIBRARIES}/glm-0.9.9.3/glm)
    set(GLFW_DIR $ENV{EXTERNAL_LIBRARIES}/glfw-3.2.1.bin.WIN64/)
    find_library(VULKAN_SDK NAMES vulkan vulkan-1 HINTS $ENV{VK_SDK_PATH}/x86_64/lib)
    pkg_search_module(GLFW REQUIRED glfw3)
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

if(WIN32)

    target_include_directories(${PROJECT_NAME} PRIVATE
    $ENV{VK_SDK_PATH}/Include
    ${INCLUDES_PATH}
    ${PLATFORM_PATH}
    ${GLM_INCLUDE_DIR}
    ${GLFW_DIR}/include
    )

endif(WIN32)

if(UNIX)
    target_include_directories(${PROJECT_NAME} PRIVATE
    $ENV{VK_SDK_PATH}/x86_64/include
    ${INCLUDES_PATH}
    ${PLATFORM_PATH}
    )
endif(UNIX)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${VULKAN_SDK}
    $ENV{GLM_INCLUDE_DIRS}
    ${GLFW_LIB}
    )