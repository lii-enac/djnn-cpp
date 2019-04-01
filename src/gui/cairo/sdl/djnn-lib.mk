local_dir := $(src_dir)/gui/cairo/sdl

lib_cppflags += -I$(local_dir)
#-I$(local_dir)/khronos
lib_cflags += -I$(local_dir)

srcs := $(shell find $(local_dir) -name "*.cpp" -not -name "*gl*")

lib_srcs +=  $(srcs)
#$(ext-srcs)
