lib_srcs += src/gui/abstract_gobj.cpp src/gui/abstract_gshape.cpp src/gui/gui.cpp
#src/gui/window.cpp
lib_srcs += $(shell find src/gui/interaction -name "*.cpp")
lib_srcs += $(shell find src/gui/picking -name "*.cpp")
lib_srcs += $(shell find src/gui/shapes -name "*.cpp")
lib_srcs += $(shell find src/gui/style -name "*.cpp")
lib_srcs += $(shell find src/gui/transformation -name "*.cpp")
lib_srcs += $(shell find src/gui/XML -name "*.cpp")
lib_srcs += $(shell find src/gui/widgets -name "*.cpp")

ifeq ($(graphics),QT)
include src/gui/qt/djnn-lib.mk
endif

ifeq ($(graphics),CAIRO)
include src/gui/cairo/djnn-lib.mk
endif

lib_djnn_deps = display core

