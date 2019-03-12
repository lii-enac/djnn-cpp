lib_djnn_deps = core
lib_srcs := $(shell find src/base -name "*.cpp")

ifeq ($(graphics),QT)
include src/gui/qt/djnn-lib-flags.mk
endif
