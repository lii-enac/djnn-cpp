lib_cppflags += -DDJNN_SDL -DDJNN_USE_SDL_THREAD=1

ifneq ($(os),em)
lib_pkg += sdl3

ifeq ($(os),Darwin)
#lib_pkgpath = "/Users/conversy/recherche/istar/code/misc/SDL/build"
endif
ifeq ($(os),Linux)
lib_pkgpath = "/home/lii/SDL/build/local/lib/pkgconfig"
#lib_pkgpath = "/home/lii/Documents/sco/SDL/build/local/lib/pkgconfig"
endif

endif
