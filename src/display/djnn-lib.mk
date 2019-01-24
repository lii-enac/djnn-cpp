lib_djnn_deps = core
lib_srcs := src/display/display.cpp

ifeq ($(os),Linux)
	lib_srcs += $(shell find src/display/linux -name "*.cpp")
	lib_ldflags = -ludev -ldrm
	lib_cppflags = -I/usr/include/drm
else
	lib_srcs += src/display/alt/alt_display.cpp
endif