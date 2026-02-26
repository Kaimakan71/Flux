/*
 * Status code utilities.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <string_view>

namespace Flux {

typedef enum {
    success = 0,
    unknownError,
    invalidArgument,
    hostAllocationFailed,
    deviceAllocationFailed,
    incompatibleDriver,
    notImplemented,
} Status;

const char *getStatusName(Status status);
const char *getStatusDescription(Status status);

}
