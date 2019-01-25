lib_cppflags = -I. -I$(LOCALDIR)/include `pkg-config --cflags sdl2 cairo pango pangocairo` -DDJNN_SDL
lib_ldflags = `pkg-config --libs sdl2 cairo pango pangocairo`

lib_srcs += $(shell find src/gui/sdl-cairo -name "*.cpp")

