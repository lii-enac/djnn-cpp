lib_djnn_deps = exec_env core
lib_srcs += src/display/display.cpp src/display/window.cpp src/display/update_drawing.cpp

ifeq ($(display),DRM)
lib_srcs += $(shell find src/display/drm -name "*.cpp")
lib_cflags += `pkg-config --cflags libdrm`
lib_ldflags += `pkg-config --libs libdrm libudev`
endif

ifeq ($(display),QT)
include src/display/qt/djnn-lib.mk
lib_srcs += src/display/alt/alt_display.cpp
endif

ifeq ($(display),SDL)
include src/display/sdl/djnn-lib.mk
lib_srcs += src/display/alt/alt_display.cpp
endif

