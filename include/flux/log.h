/*
 * Logging utilities.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#ifndef _FLUX_LOG_H
#define _FLUX_LOG_H

void __fluxLogDebug(const char *function, const char *format, ...);
#if defined(FLUX_LOG_DEBUG_ENABLED)
#define fluxLogDebug(...) __fluxLogDebug(__FUNCTION__, __VA_ARGS__)
#else
#define fluxLogDebug(...)
#endif

#endif /* !_FLUX_LOG_H */
