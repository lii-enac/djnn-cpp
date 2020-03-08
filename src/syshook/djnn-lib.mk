lib_djnn_deps = core

lib_srcs += src/syshook/external_source.cpp src/syshook/syshook.cpp \
			src/syshook/global_mutex.cpp src/syshook/main_loop.cpp src/syshook/exit.cpp \
			src/syshook/timer.cpp src/syshook/time_manager.cpp src/syshook/djnn_time_manager.cpp

ifeq ($(cross_prefix),g)
lib_ldflags += -latomic
endif

ifeq ($(os),$(filter $(os),Darwin Linux))
lib_srcs += src/syshook/unix/iofd.cpp
endif

ifeq ($(display),QT)
include src/display/qt/djnn-lib-flags.mk
lib_srcs += $(shell find src/syshook/qt -name "*.cpp")
endif

ifeq ($(os),FreeRTOS)
include src/syshook/freertos-cxx11/djnn-lib-flags.mk
lib_srcs += $(shell find src/syshook/freertos-cxx11 -name "*.cpp")
endif

