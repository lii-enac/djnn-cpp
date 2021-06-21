local_dir = $(src_dir)/base
lib_djnn_deps = exec_env core
lib_srcs += $(call rwildcard,$(local_dir)/,*.cpp)

# ifneq ($(os),$(filter $(os),FreeRTOS em crazyflie))
# lib_pkg += fmt
# endif
