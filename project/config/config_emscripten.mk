#verbose build
#V= , 0, 1, 2, 3, max

# *********************************
# important: use 'make static' instead of a mere 'make'
# *********************************

display := SDL
#options: QT SDL DRM
#options SDL with kmsdrm: ubuntu SDL2 is not compiled with kmsdrm enabled, compile it with the option 
#see how to recompile SDL2 with this option in the file tools/raspberry-ua-netinst.md

graphics := GL
#options: QT CAIRO GL

gl_context := legacy
#gl_context := OSMesa
#gl_context := zink
#gl_context := MGL

picking := COLOR
#option: COLOR ANALYTICAL

audio := AL
#options: AL AL_SOFT

physics := # set to ODE
phidgets := # set to PHIDGETS


build_dir := build


#devel
#djnn-pkgconf = djnn-cpp-dev
#install (github)
djnn-pkgconf =

#or use on local
#djnn_path = $(abspath ../djnn-cpp)


cookbook_app_for_make_test := button
cookbook_apps_extra :=

#smala flags
SMAFLAGS += -g
#SMAFLAGS += -gen-cleaner
#SMAFLAGS += -fastcomp
djnn_modules ?= animation comms audio gui display input files utils base exec_env core
#djnn_modules += c_api

#C++ flags 
CFLAGS_COMMON += -g #-fstandalone-debug -fno-limit-debug-info
#LDFLAGS_COMMON +=

#Sanitizer
#CFLAGS_COMMON += -fsanitize=address
#LDFLAGS_COMMON += -fsanitize=address
#CFLAGS_CK += -fsanitize=thread
#LDFLAGS_CK += -fsanitize=thread
#CFLAGS_COMMON += -fsanitize=memory
#LDFLAGS_COMMON += -fsanitize=memory

#CXXFLAGS_CK += -DDJNN_STL_DJNN=1
CXXFLAGS_CK += -O0
#CXXFLAGS_CK += -ftime-trace


# --
# cross-compile emscripten
# WEBGL_multidraw won't work with picking
# ************* BUG *********** : map and element allocator, strange result, because glMap* does not allow READ_BIT...

use_pch := no

os := em
compiler := llvm
linker := llvm

CC  := `brew --prefix emscripten`/bin/emcc
CXX := `brew --prefix emscripten`/bin/em++
AR := `brew --prefix emscripten`/bin/emar
RANLIB := `brew --prefix emscripten`/bin/emranlib
CFLAGS += -fPIC
DJNN_CXXFLAGS += -Wno-c++20-designator

DYNLIB = -r # -shared
#lib_suffix=.a #.bc # make static instead
CXXLD = $(AR)

EMFLAGS := \
-s USE_SDL=2 \
-pthread \
-gsource-map

em_ext_libs_path ?= ../../misc/emscripten/local-install

EMCFLAGS += $(EMFLAGS)
EMCFLAGS += -I$(em_ext_libs_path)/include -I$(shell brew --prefix)/opt/flex/include -I/usr/local/include #glm

CFLAGS += $(EMCFLAGS)
CFLAGS += -DGL2D_GL_CONTEXT_PROFILE_ES=1

LDFLAGS += $(EMFLAGS) \
-s USE_SDL=2 \
-s FULL_ES3 \
-s MIN_WEBGL_VERSION=2 \
-s MAX_WEBGL_VERSION=2 \
-s ASSERTIONS=2 \
-s ERROR_ON_UNDEFINED_SYMBOLS=1 \

#-s PTHREAD_POOL_SIZE=5

LDFLAGS += -L$(em_ext_libs_path)/lib

remotery_cflags := -DRMT_ENABLED=0

use_curl := no

#export PKG_CONFIG_PATH=$(rpi_sysroot)/usr/lib/$(isa_os_arch)/pkgconfig

# lib_soname_ += \
# 	-Wl,-rpath-link,$(rpi_sysroot)/lib/$(isa_os_arch):$(rpi_sysroot)/usr/lib/$(isa_os_arch) \
# 	-Wl,-rpath,$(install_dir)/djnn-cpp/build/lib

YACC := `brew --prefix`/opt/bison/bin/bison -d -Wno-conflicts-sr -Wno-conflicts-rr
LEX := `brew --prefix`/opt/flex/bin/flex
