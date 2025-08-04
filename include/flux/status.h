/*
 * Status codes.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#ifndef _FLUX_STATUS_H
#define _FLUX_STATUS_H

typedef enum {
    FLUX_STATUS_SUCCESS = 0,
    FLUX_STATUS_UNKNOWN_ERROR,
    FLUX_STATUS_INVALID_ARGUMENT,
    FLUX_STATUS_OUT_OF_HOST_MEMORY,
    FLUX_STATUS_OUT_OF_DEVICE_MEMORY,
    FLUX_STATUS_INCOMPATIBLE_DRIVER,
} FluxStatus;

#endif /* !_FLUX_STATUS_H */
