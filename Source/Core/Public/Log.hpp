/*
 * Logging utilities.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <string>
#include "Status.hpp"

#define FLUX_LOG_INFO(...) ::Flux::Log::info(::Flux::Log::_shortenPath(__builtin_FILE()), __builtin_LINE(), __func__, __VA_ARGS__)
#define FLUX_LOG_WARNING(...) ::Flux::Log::warning(::Flux::Log::_shortenPath(__builtin_FILE()), __builtin_LINE(), __func__, __VA_ARGS__)
#define FLUX_LOG_ERROR(...) ::Flux::Log::error(::Flux::Log::_shortenPath(__builtin_FILE()), __builtin_LINE(), __func__, __VA_ARGS__)
#define FLUX_LOG_STATUS(status, ...) ::Flux::Log::status(::Flux::Log::_shortenPath(__builtin_FILE()), __builtin_LINE(), __func__, (status), __VA_ARGS__)
#define FLUX_LOG_DEBUG(...) FLUX_LOG_INFO(__VA_ARGS__)

namespace Flux::Log {

static constexpr const char *const _shortenPath(const char *path) {
    std::string_view view = std::string_view(path);
    return view.substr(view.find("Source") + std::string::traits_type::length("Source") + 1).data();
}

void info(const char *file, unsigned int line, const char *function, const char *format, ...);
void warning(const char *file, unsigned int line, const char *function, const char *format, ...);
void error(const char *file, unsigned int line, const char *function, const char *format, ...);
void status(const char *file, unsigned int line, const char *function, Flux::Status status, const char *format, ...);

}
