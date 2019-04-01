ifneq ($(cross_prefix),em)
lib_cppflags += `pkg-config --cflags sdl2` -DDJNN_SDL
lib_ldflags += `pkg-config --libs sdl2`
else
lib_cppflags += -DDJNN_SDL	
endif
