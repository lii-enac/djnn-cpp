lib_cppflags += -DDJNN_USE_QT_MAINLOOP=1
lib_cppflags += -I$(src_dir)/.. # for moc

ifeq ($(os),Darwin)
lib_pkg +=  Qt5OpenGL Qt5Gui
qt_dir = $(shell brew --prefix qt@5)
lib_pkgpath += $(qt_dir)/lib/pkgconfig
moc := $(qt_dir)/bin/moc
endif

ifeq ($(os),Linux)
lib_pkg = Qt5OpenGL Qt5Gui
#moc := moc
endif

ifeq ($(os),MinGW)
lib_pkg = Qt5OpenGL Qt5Core
#moc := moc
endif
