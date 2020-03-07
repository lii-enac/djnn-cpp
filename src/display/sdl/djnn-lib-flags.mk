lib_cppflags += -DDJNN_SDL

ifneq ($(cross_prefix),em)
lib_cppflags += -DDJNN_SDL
lib_pkg += sdl2 SDL2_image
endif
