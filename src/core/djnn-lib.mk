lib_ldflags = -lexpat -lcurl -lpthread
lib_srcs := src/core/syshook/external_source.cpp src/core/syshook/syshook.cpp \
			src/core/syshook/main_loop.cpp \
			src/core/syshook/timer.cpp src/core/core.cpp \
			src/core/error.cpp src/core/utils-dev.cpp src/core/uri.cpp

lib_srcs += $(shell find src/core/control -name "*.cpp")
lib_srcs += $(shell find src/core/tree -name "*.cpp")
lib_srcs += $(shell find src/core/execution -name "*.cpp")
lib_srcs += $(shell find src/core/serializer -name "*.cpp")
lib_srcs += $(shell find src/core/xml -name "*.cpp")

ifeq ($(os),$(filter $(os),Darwin Linux))
lib_srcs += src/core/syshook/unix/iofd.cpp
endif

ifeq ($(graphics),QT)
include src/gui/qt/djnn-lib-flags.mk
endif

