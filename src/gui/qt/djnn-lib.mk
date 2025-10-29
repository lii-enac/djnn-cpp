include src/display/qt/djnn-lib-flags.mk #already included in core
include src/gui/qt/djnn-lib-srcs.mk
#include src/gui/qt/qt/djnn-lib-srcs.mk


ifeq ($(pkgcmdtype),apt)
ifeq ($(display),QT)
	pkglibdeps += qt5opengl5
	#pkgdeps += qt5-default #for ubuntu < 22.04
endif
pkglibdeps_full := $(addprefix lib,$(pkglibdeps))
pkgdeps += $(pkglibdeps_full)
endif


ifeq ($(pkgcmdtype),apk)
ifeq ($(display),QT)
	pkglibdeps += qt5opengl5
endif
pkglibdeps_full := $(pkglibdeps)
pkgdeps += $(pkglibdeps_full)
endif

ifeq ($(pkgcmdtype),brew)
ifeq ($(graphics),QT)
	pkgdeps += qt6
endif
endif

ifeq ($(pkgcmdtype),pacman)
	ifeq ($(graphics),QT)
		mgwpkgdeps += qt5
	endif
	mgwpkgdeps := $(addprefix mingw-w64-x86_64-, $(mgwpkgdeps))
	pkgdeps += $(mgwpkgdeps)
endif
