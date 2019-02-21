lib_cppflags = -I. -I$(LOCALDIR)/include `pkg-config --cflags sdl2 cairo pango pangocairo` -DDJNN_SDL
lib_ldflags = `pkg-config --libs sdl2 cairo pango pangocairo`

# cairo opengl
# compile cairo with: (1. mac or 2. linux)
#		env PKG_CONFIG_PATH=`pwd` ./configure --disable-dependency-tracking --enable-gobject=yes --enable-xcb=no --enable-xlib=no --enable-xlib-xrender=no --enable-gl=yes
#		env PKG_CONFIG_PATH=`pwd` ./configure --disable-dependency-tracking --enable-gobject=yes --enable-gl=yes
	
# cairo_path := /Users/conversy/tmp/cairo-1.16.0
# cairo_lib_path := $(cairo_path)/src/.libs
# lib_cppflags = -I. `pkg-config --cflags pangocairo` -I$(cairo_path) -I$(cairo_path)/cairo -I$(LOCALDIR)/include `pkg-config --cflags sdl2 pango` -DDJNN_SDL
# lib_ldflags = -L$(cairo_lib_path) -static -lcairo -L/usr/local/Cellar/pango/1.42.4/lib -L/usr/local/Cellar/glib/2.58.3/lib -L/usr/local/opt/gettext/lib \
#         -lSDL2 -lpangocairo-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lintl -Wl,-framework -Wl,CoreFoundation

lib_srcs += $(shell find src/gui/sdl-cairo -name "*.cpp")

