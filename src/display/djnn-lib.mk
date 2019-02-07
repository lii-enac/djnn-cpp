lib_djnn_deps = core
lib_srcs := src/display/display.cpp

ifeq ($(os),Linux)
	lib_srcs += $(shell find src/display/linux -name "*.cpp")
	lib_ldflags = `pkg-config --libs libdrm libudev`
	lib_cppflags = `pkg-config --cflags libdrm`
else
	lib_srcs += src/display/alt/alt_display.cpp
endif