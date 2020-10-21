lib_cppflags += -DDJNN_USE_QT_MAINLOOP=1
lib_cppflags += -I$(src_dir)/.. # for moc

ifeq ($(os),Darwin)
lib_pkg += Qt5OpenGL Qt5Gui
#moc := /usr/local/opt/qt/bin/moc
qt_dir ?= $(shell ls -dt1 /usr/local/Cellar/qt/* | head -1)
lib_pkgpath += $(qt_dir)/lib/pkgconfig
endif

ifeq ($(os),Linux)
lib_pkg = Qt5OpenGL Qt5Gui
#moc := moc
endif

ifeq ($(os),MinGW)
lib_pkg = Qt5OpenGL Qt5Core
#moc := moc
endif
