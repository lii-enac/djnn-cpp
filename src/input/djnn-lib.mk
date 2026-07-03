lib_djnn_deps += base core
local_dir = $(src_dir)/input
lib_srcs += $(local_dir)/input.cpp

use_gpio ?= no

ifeq ($(os),Linux)
	lib_srcs += $(call rwildcard,src/input/linux/,*.cpp)
	lib_djnn_deps += exec_env
	lib_ldflags += -ludev
	lib_pkg += libevdev

ifneq ($(use_gpio),no)
	lib_pkg += libgpiod
else
	lib_srcs := $(filter-out src/input/linux/linux_gpio.cpp, $(lib_srcs))
endif

else
	lib_srcs += $(local_dir)/alt/alt_input.cpp
endif
