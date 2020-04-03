lib_djnn_deps = exec_env core
local_dir := $(src_dir)/comms

lib_srcs := $(local_dir)/comms.cpp $(local_dir)/IvyAccess.cpp

# Ivy dependancy
# svn co https://svn.tls.cena.fr/svn/ivy/ivy-c/trunk ivy-c
# cd ivy-c/src
# ----------------
# MACOS
# brew tap lii-enac/repo
# brew install libivy
# or -- from source
# make -f Makefile.osx static-libs shared-libs
# sudo make -f Makefile.osx install
# ----------------
# linux
# dpkg -i libivy.deb
# or -- from source
# make static-libs shared-libs
# sudo make install

ifeq ($(os),Darwin)
lib_cppflags = -I/opt/local/include/Ivy
lib_ldflags = -L/opt/local/lib -livy
#TODO : activate after confinement and put above AND REMOVE /ivy
lib_cppflags += $(shell pkg-config ivy-c --cflags)/Ivy
lib_ldflags += $(shell pkg-config ivy-c --libs)

lib_srcs += $(local_dir)/serial.cpp
endif
ifeq ($(os),Linux)
# ODO: remove once .pkg made 
lib_cppflags = -I/usr/local/include/Ivy
lib_ldflags = -L/usr/local/lib64 -livy -lpcre
lib_srcs += $(local_dir)/serial.cpp
endif
ifeq ($(os),MinGW)
# TODO: remove once .pkg made 
lib_cppflags = -I/usr/include
lib_ldflags = -L/usr/lib -livy -lws2_32 -L/mingw64/lib -lpcre
endif

# library-specific thread support
ifeq ($(display),QT)
include src/display/qt/djnn-lib-flags.mk
endif

ifeq ($(display),SDL)
include src/display/sdl/djnn-lib-flags.mk
endif



#ifeq ($(os),FreeRTOS)
#include src/exec_env/freertos-cxx11/djnn-lib-flags.mk
#endif



