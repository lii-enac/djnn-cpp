lib_djnn_deps = core
local_dir = $(src_dir)/input
lib_srcs += $(local_dir)/input.cpp

ifeq ($(os),Linux)
	lib_srcs += $(call rwildcard,src/input/linux/,*.cpp)
	lib_cppflags = -I/usr/include/libevdev-1.0
	lib_ldflags = -ludev -levdev
else
	lib_srcs += $(local_dir)/alt/alt_input.cpp
endif
