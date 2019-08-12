ifneq ($(cross_prefix),em)
lib_cppflags += `pkg-config --cflags sdl2 SDL2_image` -DDJNN_SDL
lib_ldflags += `pkg-config --libs sdl2 SDL2_image`
else
lib_cppflags += -DDJNN_SDL
endif
