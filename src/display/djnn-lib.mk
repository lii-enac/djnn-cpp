lib_djnn_deps = core
lib_srcs += src/display/display.cpp src/display/window.cpp src/display/update_drawing.cpp

ifeq ($(os),Linux)
lib_srcs += $(shell find src/display/linux -name "*.cpp")
lib_cflags += `pkg-config --cflags libdrm`
lib_ldflags += `pkg-config --libs libdrm libudev`
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

