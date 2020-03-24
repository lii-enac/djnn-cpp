lib_djnn_deps = core
lib_srcs := src/input/input.cpp

lib_headers := $(shell find src/input -name "*.h")

ifeq ($(os),Linux)
	lib_srcs += $(shell find src/input/linux -name "*.cpp")
	lib_cppflags = -I/usr/include/libevdev-1.0
	lib_ldflags = -ludev -levdev
else
	lib_srcs += src/input/alt/alt_input.cpp
endif
