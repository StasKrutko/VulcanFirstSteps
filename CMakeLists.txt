cmake_minimum_required(VERSION 3.10)
project(VulkanTest C CXX)
include(FindPkgConfig)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DGLFW_INCLUDE_VULKAN -DNOMINMAX)

set(SOURCES_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDES_PATH ${SOURCES_PATH}/include)

if(WIN32)
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR -DGLFW_EXPOSE_NATIVE_WIN32)

    set(PLATFORM_PATH ${SOURCES_PATH}/windows)
    set(GLM_INCLUDE_DIR $ENV{EXTERNAL_LIBRARIES}/glm-0.9.9.3/glm)
    set(GLFW_DIR $ENV{EXTERNAL_LIBRARIES}/glfw-3.2.1.bin.WIN64)

    find_library(VULKAN_LIB NAMES vulkan vulkan-1 HINTS $ENV{VK_SDK_PATH}/Lib)
    find_library(GLFW_LIB NAMES glfw3 HINTS ${GLFW_DIR}/lib-vc2015)

endif(WIN32)

if(UNIX)
    add_definitions(-DVK_USE_PLATFORM_XCB_KHR -DGLFW_EXPOSE_NATIVE_X11)

    set(PLATFORM_PATH ${SOURCES_PATH}/linux)
    set(GLFW_DIR $ENV{EXTERNAL_LIBRARIES}/GLFW/glfw-3.2.1)

    find_library(VULKAN_LIB NAMES vulkan vulkan-1 HINTS $ENV{VK_SDK_PATH}/x86_64/lib)
    pkg_search_module(GLFW REQUIRED glfw3)
endif(UNIX)

add_executable(${PROJECT_NAME} "${SOURCES}")

target_sources(${PROJECT_NAME}
    PUBLIC
    ${SOURCES_PATH}/HelloTriangleApplication.cpp
    ${SOURCES_PATH}/main.cpp
    ${INCLUDES_PATH}/Getting_started.hpp
    ${INCLUDES_PATH}/HelloTriangleApplication.hpp
    ${PLATFORM_PATH}/HelloTriangle_platform.hpp
#    ${SOURCES_PATH}/TutorialExample.cpp
)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${INCLUDES_PATH}
        ${PLATFORM_PATH}
    )

if(WIN32)
    target_include_directories(${PROJECT_NAME}
        PRIVATE
        $ENV{VK_SDK_PATH}/Include
        ${GLM_INCLUDE_DIR}
        ${GLFW_DIR}/include
    )

endif(WIN32)

if(UNIX)
    target_include_directories(${PROJECT_NAME}
        PRIVATE
        $ENV{VK_SDK_PATH}/x86_64/include
    )
endif(UNIX)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    ${VULKAN_LIB}
    ${GLFW_LIB}
    ${GLFW_STATIC_LIBRARIES}
)
