lib_cppflags += -DDJNN_SDL

ifneq ($(os),em)
lib_pkg += sdl3
#lib_pkgpath = "/Users/conversy/recherche/istar/code/misc/SDL/build"
#lib_pkg += sdl3-dev
#SDL2_image
endif
