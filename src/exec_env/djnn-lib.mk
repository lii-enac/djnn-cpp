lib_djnn_deps = core

lib_srcs += src/exec_env/external_source.cpp src/exec_env/exec_env.cpp \
			src/exec_env/global_mutex.cpp src/exec_env/main_loop.cpp src/exec_env/exit.cpp \
			src/exec_env/timer.cpp src/exec_env/time_manager.cpp src/exec_env/djnn_time_manager.cpp

ifeq ($(cross_prefix),g)
lib_ldflags += -latomic
endif

ifeq ($(os),$(filter $(os),Darwin Linux))
lib_srcs += src/exec_env/unix/iofd.cpp
endif

ifeq ($(display),QT)
include src/display/qt/djnn-lib-flags.mk
lib_srcs += $(shell find src/exec_env/qt -name "*.cpp")
endif

ifeq ($(display),SDL)
include src/display/sdl/djnn-lib-flags.mk
endif

ifeq ($(os),FreeRTOS)
include src/exec_env/freertos-cxx11/djnn-lib-flags.mk
lib_srcs += $(shell find src/exec_env/freertos-cxx11 -name "*.cpp")
endif
