lib_djnn_deps = exec_env core
local_dir := $(src_dir)/comms
lib_srcs += $(local_dir)/comms.cpp $(local_dir)/remote_proc.cpp $(local_dir)/proc_exporter.cpp $(local_dir)/socket.cpp
ifeq ($(use_ivy),yes)
lib_srcs += $(local_dir)/IvyAccess.cpp
lib_cppflags += -Wno-invalid-utf8
endif
#lib_pkg += rtmidi

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
# 	lib_cppflags += -I/opt/local/include/Ivy
# 	lib_ldflags += -L/opt/local/lib -livy
	
	#TODO : activate after confinement and put above AND REMOVE /ivy

	ifeq ($(use_ivy),yes)
	tmp_lib_cppflags = $(shell PKG_CONFIG_PATH=$(comms_lib_pkgpath) $(PKG_CONFIG) libivy --cflags)
	ifneq ($(tmp_lib_cppflags),)
		lib_cppflags += $(shell PKG_CONFIG_PATH=$(comms_lib_pkgpath) $(PKG_CONFIG) libivy --cflags)/Ivy
	endif
	lib_ldflags += $(shell PKG_CONFIG_PATH=$(comms_lib_pkgpath) $(PKG_CONFIG) libivy --libs)
	endif

	lib_srcs += $(local_dir)/serial.cpp
endif

ifeq ($(os),Linux)
	ifeq ($(use_ivy),yes)
	pkgdeps += libpcre3-dev
	# TODO: add support for pkg-config
	# TODO: remove once .pkg made 
	lib_cppflags += -I/usr/local/include/Ivy
	lib_ldflags += -L/usr/local/lib -livy -lpcre
	endif

	lib_srcs += $(local_dir)/serial.cpp
endif

ifeq ($(os),MinGW)
	ifeq ($(use_ivy),yes)
	# TODO: add support for pkg-config
	# TODO: remove once .pkg made 
	lib_cppflags += -I/usr/include/Ivy
	lib_ldflags += -L/usr/lib -livy -lws2_32 -L/mingw64/lib -lpcre
	else
	lib_ldflags += -L/usr/lib -lws2_32
	endif
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

# ifneq ($(os),$(filter $(os),FreeRTOS em crazyflie))
# lib_pkg += fmt
# endif
