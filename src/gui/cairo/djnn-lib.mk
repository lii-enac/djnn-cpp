lib_srcs += $(shell find src/gui/cairo -name "*.cpp" -not -path "*gl*")

lib_cppflags += -I. -I$(LOCALDIR)/include `pkg-config --cflags cairo pango pangocairo` -DDJNN_SDL
lib_ldflags += `pkg-config --libs cairo pango pangocairo`

# cairo opengl	
#cairo_path := /Users/conversy/tmp/cairo-1.16.0
#cairo_path := /home/lii/Documents/cairo
# cairo_lib_path := $(cairo_path)/src/.libs
# lib_srcs += src/gui/cairo/cairo_gl_sdl_window.cpp
# lib_cppflags += -I$(cairo_path) -I$(cairo_path)/cairo `pkg-config --cflags pangocairo` `pkg-config --cflags pango` -DUSE_GL_ES_VERSION_2_0=1 -Isrc/gui/gl_
# lib_ldflags += -L$(cairo_lib_path) -lcairo -L/usr/local/Cellar/pango/1.42.4/lib -L/usr/local/Cellar/glib/2.58.3/lib -L/usr/local/opt/gettext/lib \
#          -lpangocairo-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0
# mac:
# -lintl -Wl,-framework -Wl,CoreFoundation -L/Users/conversy/src-ext/SwiftShader/build -lEGL 

include src/gui/sdl/djnn-lib.mk
#temporary
include src/gui/gl_/djnn-lib.mk


# for cairo opengl
# compile cairo with: (1. mac or 2. linux)
#		env PKG_CONFIG_PATH=`pwd` ./configure --disable-dependency-tracking --enable-gobject=yes --enable-xcb=no --enable-xlib=no --enable-xlib-xrender=no --enable-gl=yes
#		env PKG_CONFIG_PATH=`pwd` ./configure --disable-dependency-tracking --enable-gobject=yes --enable-gl=yes
#		env PKG_CONFIG_PATH=`pwd` ./configure --disable-dependency-tracking --enable-xcb=no --enable-xlib=no --enable-xlib-xrender=no --enable-glesv2=yes --enable-egl=yes --enable-quartz=yes --enable-script=no

#		linux:
#		./configure --enable-gl=yes --enable-glx=yes
