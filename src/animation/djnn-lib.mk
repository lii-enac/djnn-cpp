lib_djnn_deps = core base
local_dir := $(src_dir)/animation
lib_srcs += $(call rwildcard,$(local_dir)/,*.cpp)