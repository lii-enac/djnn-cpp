#lib_cppflags += -DDJNN_USE_QT_MAINLOOP=1
lib_cppflags += -I$(src_dir)/.. # for moc

ifeq ($(os),Darwin)
#qt5
qt_version := 5
lib_pkg +=  Qt5OpenGL Qt5Gui
qt_dir = $(brew_prefix)/opt/qt@5
lib_pkgpath += $(qt_dir)/lib/pkgconfig
moc ?= $(qt_dir)/bin/moc

#qt6
# qt_version := 6
# lib_pkg +=  Qt6OpenGL Qt6Gui Qt6Widgets
# qt_dir = $(brew_prefix)/opt/qt
# lib_pkgpath += $(qt_dir)/libexec/lib/pkgconfig
# moc := $(shell qmake -query QT_HOST_LIBEXECS)/moc

endif

ifeq ($(os),Linux)
lib_pkg = Qt5OpenGL Qt5Gui
moc ?= moc
endif

ifeq ($(os),MinGW)
lib_pkg = Qt5OpenGL Qt5Core
moc ?= moc
endif
