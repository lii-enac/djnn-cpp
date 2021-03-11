lib_cppflags += -DDJNN_USE_QT_MAINLOOP=1
lib_cppflags += -I$(src_dir)/.. # for moc

ifeq ($(os),Darwin)
lib_pkg +=  Qt5OpenGL Qt5Gui
qt_dir = $(shell ls -dt1 /usr/local/Cellar/qt/5* | head -1)
ifneq ($(qt_dir),-1)
qt_dir = $(shell ls -dt1 /usr/local/Cellar/qt@5/* | head -1)
endif
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
