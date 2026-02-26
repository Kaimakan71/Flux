# Flux
Flux is a performance-focused game engine written in C++. Flux uses GLFW to manage its windows and supports rendering with OpenGL and Vulkan. Flux's functionality is divided into the `Core` and `RHI` subsystems.

## Core
The `Core` subsystem is responsible for handling logging and managing windows. The header `Log.hpp` provides utilities like `FLUX_LOG_ERROR` which should be used to report the status of the application to aid in debugging and profiling. `Window.hpp` provides the `Window` class, which can be used to create windows using GLFW.

## RHI (Rendering Hardware Interface)
The `RHI` subsystem is responsible for interfacing with the graphics hardware and providing a higher-level API that requires minimal application code to handle differences between low-level rendering APIs.

### RHI Implementation Status
| Feature            | OpenGL | Vulkan |
|--------------------|--------|--------|
| `RHI`              | ✅     | ✅     |
| `RHIDevice`        | ✅     | ✅     |
| `RHIPipeline`      | ❌     | ✅     |
| `RHICommandPool`   | ❌     | ☑️     |
| `RHICommandBuffer` | ❌     | ❌     |
* ❌: not implemented.
* ☑️: partially implemented.
* ✅: fully implemented (features may still be added in future versions).
