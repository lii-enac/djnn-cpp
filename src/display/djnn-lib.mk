lib_djnn_deps = core
lib_srcs += src/display/display.cpp src/display/window.cpp
#lib_srcs += $(shell find src/display/picking -name "*.cpp")

ifeq ($(os),Linux)
lib_srcs += $(shell find src/display/linux -name "*.cpp")
lib_ldflags += `pkg-config --libs libdrm libudev`
lib_cppflags += `pkg-config --cflags libdrm`
endif

ifeq ($(display),QT)
include src/display/qt/djnn-lib.mk
endif

ifeq ($(display),SDL)
include src/display/sdl/djnn-lib.mk
endif

ifeq ($(display),ALT)
lib_srcs += src/display/alt/alt_display.cpp
endif

#ifeq ($(graphics),GL)
#include src/display/gl/djnn-lib.mk
#endif

