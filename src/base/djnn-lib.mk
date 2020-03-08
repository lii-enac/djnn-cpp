lib_djnn_deps = exec_env core
lib_srcs := $(shell find src/base -name "*.cpp")

ifeq ($(display),QT)
include src/display/qt/djnn-lib-flags.mk #already included in core
endif

ifneq (,$(filter $(graphics),CAIRO GL))
include src/display/sdl/djnn-lib-flags.mk
endif

ifeq ($(os),FreeRTOS)
include src/core/exec_env/freertos-cxx11/djnn-lib-flags.mk
endif

