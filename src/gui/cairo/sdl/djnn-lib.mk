local_dir := $(src_dir)/gui/cairo/sdl
lib_cflags += -I$(local_dir)
lib_srcs += $(wildcard $(local_dir)/cairo_sdl*.cpp)
