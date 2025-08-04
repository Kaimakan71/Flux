# Flux

Flux is a "game engine" which currently consists of an RHI (Rendering Hardware Interface) supporting OpenGL and Vulkan.

# API

## `FluxRhi` (`flux/rhi.h`)

Flux's RHI (Rendering Hardware Interface) is a layer over OpenGL and Vulkan that allows the application to use virtually the same rendering code no matter which underlying API the RHI is using.

### Notes

* Each application should only create one `FluxRhi`.
* `fluxRhiInitialize()` should not be called until *after* the application has created its window (and its context, if using OpenGL).
