lib_djnn_deps = core
lib_srcs += src/input/input.cpp

ifeq ($(os),Linux)
	lib_srcs += $(call rwildcard,src/input/linux/,*.cpp)
	lib_cppflags = -I/usr/include/libevdev-1.0
	lib_ldflags = -ludev -levdev
else
	lib_srcs += src/input/alt/alt_input.cpp
endif
