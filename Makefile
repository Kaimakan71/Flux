# Flux build configuration.
# Copyright (c) 2025, Quinn Stephens.
# All rights reserved.
# Provided under the BSD 3-Clause license.

# Project configuration
BUILD_DEBUG_ENABLED = 0
RUNTIME_DEBUG_ENABLED = 1
SRCDIR = src
INCDIR = include
OBJDIR = build/obj
BINDIR = build/bin
CFILES = \
	$(addprefix $(SRCDIR)/rhi/opengl/utils/,strings.c log.c loader.c) \
	$(addprefix $(SRCDIR)/rhi/vulkan/utils/,strings.c log.c loader.c wrappers.c instance.c device.c swapchain.c) \
	$(addprefix $(SRCDIR)/,log.c) \
	$(addprefix $(SRCDIR)/rhi/opengl/,device.c rhi.c) \
	$(addprefix $(SRCDIR)/rhi/vulkan/,device.c rhi.c) \
	$(addprefix $(SRCDIR)/rhi/,rhi.c) \
	$(addprefix $(SRCDIR)/example/,main.c)
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS =

# Build debugging configuration
ifeq ($(BUILD_DEBUG_ENABLED),1)
CFLAGS += -g
LDFLAGS += -g
else
CFLAGS += -O3
LDFLAGS += -flto=thin
endif

# Runtime debugging configuration
ifeq ($(RUNTIME_DEBUG_ENABLED),1)
CFLAGS += -DFLUX_DEBUG_ENABLED -DVK_UTILS_DEBUG_ENABLED -DGL_UTILS_DEBUG_ENABLED
endif

# System-specific configuration
ifeq ($(OS),Windows_NT)
CC = x86_64-w64-mingw32-clang
LD = x86_64-w64-mingw32-ld
LDFLAGS += -lglfw3
EXENAME = example.exe
else
CC = clang
LD = ld.lld
LDFLAGS += -lglfw
EXENAME = example
endif

OFILES = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(CFILES))
EXEFILE = $(BINDIR)/$(EXENAME)

.PHONY: all
all: $(EXEFILE)

$(EXEFILE): $(OFILES) | $(BINDIR)
	@echo Linking $@...
	@$(CC) $^ $(LDFLAGS) -o "$@"

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@echo Compiling $<...
ifeq ($(OS),Windows_NT)
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
else
	@mkdir -p "$(dir $@)"
endif
	@$(CC) -c "$<" $(CFLAGS) -o "$@"

$(OBJDIR) $(BINDIR):
ifeq ($(OS),Windows_NT)
	@mkdir "$@"
else
	@mkdir -p "$@"
endif

.PHONY: clean
clean:
	@echo Cleaning...
ifeq ($(OS),Windows_NT)
	@-rmdir /s /q "$(BINDIR)" 2>nul
	@-rmdir /s /q "$(OBJDIR)" 2>nul
else
	@rm -rf $(BINDIR) $(OBJDIR)
endif
