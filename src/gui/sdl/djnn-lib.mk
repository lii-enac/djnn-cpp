ifneq ($(cross_prefix),em)
lib_cppflags += `pkg-config --cflags sdl2` -DDJNN_SDL
lib_ldflags += `pkg-config --libs sdl2`
endif

lib_srcs += $(shell find src/gui/sdl -name "*.cpp")
