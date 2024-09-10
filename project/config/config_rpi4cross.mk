#verbose build
#V= , 0, 1, 2, 3, max

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

use_ivy := no

djnn_libs_extra :=

# optimization, the higher the level, the faster runtime, the slower compile
# CFLAGS += -O0
# CFLAGS += -O1 # recommended with sanitizer
# CFLAGS += -O2
CFLAGS += -O0

# debug, compile is slower with it
CFLAGS += -g
#CFLAGS += -fstandalone-debug

# sanitizer
#CFLAGS += -fsanitize=address
#LDFLAGS += -fsanitize=address
#CFLAGS += -fsanitize=thread
#LDFLAGS += -fsanitize=thread

CFLAGS += -Wno-gnu #-null-pointer-arithmetic
#CFLAGS += -fno-limit-debug-info -fstandalone-debug
#LDFLAGS += -fno-limit-debug-info -fstandalone-debug


build_dir := build

remotery_cflags += -DRMT_ENABLED=1
remotery_cflags += -DRMT_USE_OPENGL=1

keep_intermediate:=yes

# -----
# cross-compile rpi4 debian kmsdrm

use_pch := no

os := Linux
compiler := llvm
linker := gnu

rpi_sysroot := /Users/conversy/recherche/istar/code/misc/rpi_home/sysroot
isa_os_arch := arm-linux-gnueabihf
isa := arm
cc_version := 10
install_dir := /root

CC  := `brew --prefix llvm`/bin/clang
CXX := `brew --prefix llvm`/bin/clang++
CFLAGS  += --target=$(isa_os_arch)
CFLAGS += -fPIC
DJNN_CXXFLAGS += -Wno-c++20-designator
LDFLAGS += --target=$(isa_os_arch)

export PKG_CONFIG_PATH=$(rpi_sysroot)/usr/lib/$(isa_os_arch)/pkgconfig

CFLAGS += \
	--sysroot=$(rpi_sysroot) \
    -isysroot $(rpi_sysroot) \
    -isystem $(rpi_sysroot)/usr/include/c++/$(cc_version) \
    -isystem $(rpi_sysroot)/usr/include/$(isa_os_arch)/c++/$(cc_version)

#	-isystem $(rpi_sysroot)/usr/include/$(isa_os_arch)

LDFLAGS += \
	--sysroot=$(rpi_sysroot) \
	-L$(rpi_sysroot)/usr/lib/gcc/$(isa_os_arch)/$(cc_version) \
    -B$(rpi_sysroot)/usr/lib/gcc/$(isa_os_arch)/$(cc_version)

#--sysroot=$(rpi_sysroot) \

lib_soname_ += \
	-Wl,-rpath-link,$(rpi_sysroot)/lib/$(isa_os_arch):$(rpi_sysroot)/usr/lib/$(isa_os_arch) \
	-Wl,-rpath,$(install_dir)/djnn-cpp/build/lib

YACC := `brew --prefix`/opt/bison/bin/bison -d -Wno-conflicts-sr -Wno-conflicts-rr
LEX := `brew --prefix`/opt/flex/bin/flex


CFLAGS += -I$(rpi_sysroot)/usr/include/libevdev-1.0

ifeq ($(display),DRM)
CFLAGS += -I$(rpi_sysroot)/usr/include/libdrm
endif



# rpi_sysroot := /Users/conversy/recherche/istar/code/misc/jetson_home/sysroot.jetson
# isa_os_arch := aarch64-linux-gnu
# isa := aarch64
# cc_version := 10
# install_dir := /home/conversy

# compiler := gnu
# linker := gnu
# rpi_sysroot := ./Users/conversy/recherche/istar/code/misc/rpi_home/sysroot64
# #isa_os_arch := aarch64-none-elf
# isa_os_arch := aarch64-linux-gnu
# cc_version := 12
# CC  := aarch64-none-elf-gcc
# CXX := aarch64-none-elf-g++

# compiler := gnu
# linker := gnu
# rpi_sysroot := /Users/conversy/recherche/istar/code/misc/rpi_home/sysroot64
# #isa_os_arch := aarch64-none-elf
# isa_os_arch := aarch64-linux-gnu
# cc_version := 12
# CC  := zig cc
# CXX := zig c++

# CFLAGS  += --target=$(isa_os_arch)
# LDFLAGS += --target=$(isa_os_arch)

# --

