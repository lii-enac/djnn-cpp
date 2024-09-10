lib_cppflags += -DDJNN_SDL

ifneq ($(os),em)
lib_pkg += sdl2
#SDL2_image
endif
