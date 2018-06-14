ifeq ($(os),Darwin)
ifeq ($(graphics),QT)
QTLIBS = -framework QtWidgets -framework QtOpenGL \
		-framework QtGui -framework QtCore
ifneq ("$(wildcard /opt/local/libexec/qt5/lib/*)","")
FRAMEWORKDIR = /opt/local/libexec/qt5/lib
else ifneq ("$(wildcard /usr/local/Cellar/qt/*)","")
FRAMEWORKDIR = $(shell ls -dt1 /usr/local/Cellar/qt/* | head -1)/Frameworks
moc := /usr/local/Cellar/qt/*/bin/moc
else
FRAMEWORKDIR = ~/Qt/Frameworks
endif
endif

QTWIDGETS = $(FRAMEWORKDIR)/QtWidgets.framework
QTOPENGL = $(FRAMEWORKDIR)/QtOpenGL.framework
QTGUI = $(FRAMEWORKDIR)/QtGui.framework
QTCORE = $(FRAMEWORKDIR)/QtCore.framework


#$(build_dir)/src/gui/qt/%.o: CXXFLAGS +=
lib_cppflags = -I. \
		-I$(LOCALDIR)/include -F$(FRAMEWORKDIR) \
		-I$(QTWIDGETS)/Headers -I$(QTOPENGL)/Headers \
		-I$(QTGUI)/Headers -I$(QTCORE)/Headers
lib_ldflags = -F$(FRAMEWORKDIR) $(QTLIBS)
endif

ifeq ($(os),Linux)
ifeq ($(graphics),QT)
QTLIBS = Qt5OpenGL Qt5Gui
moc := moc
lib_cppflags = -I. -I$(LOCALDIR)/include `pkg-config --cflags $(QTLIBS)`
lib_ldflags = `pkg-config --libs $(QTLIBS)`
endif
endif


ifeq ($(os),MINGW64_NT-10.0)
ifeq ($(graphics),QT)
QTLIBS = Qt5OpenGL Qt5Gui 
moc := moc
lib_cppflags = -I. -I$(LOCALDIR)/include $(shell pkg-config --cflags $(QTLIBS))
lib_ldflags = $(shell pkg-config --libs $(QTLIBS)) 
endif
endif

lib_srcs := src/gui/abstract_gobj.cpp src/gui/abstract_gshape.cpp src/gui/gui.cpp src/gui/window.cpp
lib_srcs += $(shell find src/gui/picking -name "*.cpp")
lib_srcs += $(shell find src/gui/shapes -name "*.cpp")
lib_srcs += $(shell find src/gui/style -name "*.cpp")
lib_srcs += $(shell find src/gui/transformation -name "*.cpp")
lib_srcs += $(shell find src/gui/XML -name "*.cpp")

ifeq ($(graphics),QT)
include src/gui/qt/djnn-lib.mk
endif

ifeq ($(graphics),SDL)
include src/gui/sdl/djnn-lib.mk
endif



lib_djnn_deps = core
