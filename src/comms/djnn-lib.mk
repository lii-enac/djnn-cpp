lib_djnn_deps = syshook core
lib_srcs := $(shell find src/comms -name "*.cpp")

# Ivy dependancy
# svn co https://svn.tls.cena.fr/svn/ivy/ivy-c/trunk ivy-c
# cd ivy-c/src
# MACOS
# make -f Makefile.osx static-libs shared-libs
# sudo make -f Makefile.osx installlibs includes
# linux
# make static-libs shared-libs
# sudo make installlibs includes

ifeq ($(os),Darwin)
lib_cppflags = -I/opt/local/include/Ivy
lib_ldflags = -L/opt/local/lib -livy
endif
ifeq ($(os),Linux)
lib_cppflags = -I/usr/local/include/Ivy
lib_ldflags = -L/usr/local/lib64 -livy -lpcre
endif
ifeq ($(os),MinGW)
lib_cppflags = -I/usr/include
lib_ldflags = -L/usr/lib -livy -lws2_32 -L/mingw64/lib -lpcre
endif

# library-specific thread support
ifeq ($(display),QT)
include src/display/qt/djnn-lib-flags.mk
#lib_srcs += $(shell find src/core/syshook/qt -name "*.cpp")
endif

ifeq ($(display),SDL)
include src/display/sdl/djnn-lib-flags.mk
endif

ifeq ($(os),FreeRTOS)
include src/core/syshook/freertos-cxx11/djnn-lib-flags.mk
lib_srcs += $(shell find src/core/syshook/freertos-cxx11 -name "*.cpp")
endif



