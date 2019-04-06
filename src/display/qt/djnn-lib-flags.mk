ifeq ($(os),Darwin)
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

QTWIDGETS = $(FRAMEWORKDIR)/QtWidgets.framework
QTOPENGL = $(FRAMEWORKDIR)/QtOpenGL.framework
QTGUI = $(FRAMEWORKDIR)/QtGui.framework
QTCORE = $(FRAMEWORKDIR)/QtCore.framework

#$(build_dir)/src/gui/qt/%.o: CXXFLAGS +=
lib_cppflags += -I. \
		-I$(LOCALDIR)/include -F$(FRAMEWORKDIR) \
		-I$(QTWIDGETS)/Headers -I$(QTOPENGL)/Headers \
		-I$(QTGUI)/Headers -I$(QTCORE)/Headers
lib_ldflags += -F$(FRAMEWORKDIR) $(QTLIBS)
endif


ifeq ($(os),Linux)
QTLIBS = Qt5OpenGL Qt5Gui
moc := moc
lib_cppflags += -I. -I$(LOCALDIR)/include `pkg-config --cflags $(QTLIBS)`
lib_ldflags += `pkg-config --libs $(QTLIBS)`
endif


ifeq ($(os),$(osmingw))
QTLIBS = Qt5OpenGL Qt5Core
moc := moc
lib_cppflags += -I. -I$(LOCALDIR)/include $(shell pkg-config --cflags $(QTLIBS))
lib_ldflags += $(shell pkg-config --libs $(QTLIBS)) 
endif
