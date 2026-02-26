# CMake build configuration.
# Copyright (c) 2025-2026, Quinn Stephens.
# All rights reserved.
# Provided under the BSD 3-Clause license.

cmake_minimum_required(VERSION 3.14)
project(Flux CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER clang++)

set(CORE_SOURCES
    Public/Status.hpp
    Private/Status.cpp
    Public/Log.hpp
    Private/Log.cpp
    Public/RenderingAPI.hpp
    Public/Window.hpp
    Private/Window.cpp
)
list(TRANSFORM CORE_SOURCES PREPEND Source/Core/)

set(RHI_OPENGL_SOURCES
    OpenGLDispatch.hpp
    OpenGLLoader.hpp
    OpenGLLoader.cpp
    OpenGLPipeline.hpp
    OpenGLPipeline.cpp
    OpenGLCommandPool.hpp
    OpenGLCommandPool.cpp
    OpenGLCommandBuffer.hpp
    OpenGLCommandBuffer.cpp
    OpenGLDevice.hpp
    OpenGLDevice.cpp
    OpenGLRHI.hpp
    OpenGLRHI.cpp
)
list(TRANSFORM RHI_OPENGL_SOURCES PREPEND Private/OpenGL/)

set(RHI_VULKAN_SOURCES
    VulkanResult.hpp
    VulkanResult.cpp
    VulkanLog.hpp
    VulkanLog.cpp
    VulkanDispatch.hpp
    VulkanLoader.hpp
    VulkanLoader.cpp
    VulkanHelpers.hpp
    VulkanHelpers.cpp
    VulkanPipeline.hpp
    VulkanPipeline.cpp
    VulkanCommandPool.hpp
    VulkanCommandPool.cpp
    VulkanCommandBuffer.hpp
    VulkanCommandBuffer.cpp
    VulkanDevice.hpp
    VulkanDevice.cpp
    VulkanRHI.hpp
    VulkanRHI.cpp
)
list(TRANSFORM RHI_VULKAN_SOURCES PREPEND Private/Vulkan/)

set(RHI_SOURCES
    Public/RHI.hpp
    Public/RHIDevice.hpp
    Public/RHIPipeline.hpp
    Public/RHICommandPool.hpp
    ${RHI_OPENGL_SOURCES}
    ${RHI_VULKAN_SOURCES}
    Private/RHI.cpp
)
list(TRANSFORM RHI_SOURCES PREPEND Source/RHI/)

set(SOURCES
    ${CORE_SOURCES}
    ${RHI_SOURCES}
    Source/main.cpp
)

add_executable(flux ${SOURCES})
target_include_directories(flux PRIVATE Source/Core/Public Source/RHI/Public)
target_link_libraries(flux PRIVATE glfw)
