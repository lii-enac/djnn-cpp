local_dir := $(src_dir)/extra/crazyflie_rc

lib_djnn_deps = core

lib_srcs += $(shell find $(local_dir))
lib_cflags += -I$(local_dir)/ext/crazyflie_cpp/include/crazyflie_cpp
lib_pkg += libusb-1.0
