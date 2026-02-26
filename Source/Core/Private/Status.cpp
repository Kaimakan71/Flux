/*
 * Status code utilities.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "Status.hpp"

namespace Flux {

const char *getStatusName(Status status) {
    switch (status) {
    case Status::success:
        return "Flux::Status::success";
    case Status::unknownError:
        return "Flux::Status::unknownError";
    case Status::invalidArgument:
        return "Flux::Status::invalidArgument";
    case Status::hostAllocationFailed:
        return "Flux::Status::hostAllocationFailed";
    case Status::deviceAllocationFailed:
        return "Flux::Status::deviceAllocationFailed";
    case Status::incompatibleDriver:
        return "Flux::Status::incompatibleDriver";
    case Status::notImplemented:
        return "Flux::Status::notImplemented";
    default:
        return "[unrecognized Flux::Status value]";
    }
}

const char *getStatusDescription(Status status) {
    switch (status) {
    case Status::success:
        return "Operation completed successfully";
    case Status::unknownError:
        return "Unknown error";
    case Status::invalidArgument:
        return "Invalid argument";
    case Status::hostAllocationFailed:
        return "Host memory allocation failed";
    case Status::deviceAllocationFailed:
        return "Device memory allocation failed";
    case Status::incompatibleDriver:
        return "Incompatible driver";
    case Status::notImplemented:
        return "Operation not implemented";
    default:
        return "[unrecognized Flux::Status value]";
    }
}

}
