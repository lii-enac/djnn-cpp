lib_cppflags += -DDJNN_USE_QT_MAINLOOP=1

ifeq ($(os),Darwin)
QTLIBS = -framework QtWidgets -framework QtOpenGL \
		-framework QtGui -framework QtCore
ifneq ("$(wildcard /usr/local/Cellar/qt/*)","")
FRAMEWORKDIR = $(shell ls -dt1 /usr/local/Cellar/qt/* | head -1)/Frameworks
moc := /usr/local/opt/qt/bin/moc
else
FRAMEWORKDIR = ~/Qt/Frameworks
endif

QTWIDGETS = $(FRAMEWORKDIR)/QtWidgets.framework
QTOPENGL = $(FRAMEWORKDIR)/QtOpenGL.framework
QTGUI = $(FRAMEWORKDIR)/QtGui.framework
QTCORE = $(FRAMEWORKDIR)/QtCore.framework

#$(build_dir)/src/gui/qt/%.o: CXXFLAGS +=
lib_cppflags += -I. \
		-F$(FRAMEWORKDIR) \
		-I$(QTWIDGETS)/Headers -I$(QTOPENGL)/Headers \
		-I$(QTGUI)/Headers -I$(QTCORE)/Headers
lib_ldflags += -F$(FRAMEWORKDIR) $(QTLIBS)
endif


ifeq ($(os),Linux)
QTLIBS = Qt5OpenGL Qt5Gui
moc := moc
lib_cppflags += -I. -I$(LOCALDIR)/include
lib_pkg += $(QTLIBS)
endif

ifeq ($(os),MinGW)
QTLIBS = Qt5OpenGL Qt5Core
moc := moc
lib_cppflags += -I. -I$(LOCALDIR)/include
lib_pkg += $(QTLIBS))
endif
