lib_ldflags +=

lib_srcs := src/core/core.cpp

lib_srcs += $(shell find src/core/ontology -name "*.cpp")
lib_srcs += $(shell find src/core/control -name "*.cpp")
lib_srcs += $(shell find src/core/tree -name "*.cpp")
lib_srcs += $(shell find src/core/execution -name "*.cpp")
lib_srcs += $(shell find src/core/serializer -name "*.cpp")
lib_srcs += $(shell find src/core/utils -name "*.cpp")

lib_srcs += src/core/syshook/external_source.cpp src/core/syshook/syshook.cpp \
			src/core/syshook/main_loop.cpp \
			src/core/syshook/timer.cpp

#ifneq ($(os),FreeRTOS)
ifneq ($(os),$(filter $(os),FreeRTOS em))
lib_srcs += $(shell find src/core/xml -name "*.cpp")
lib_ldflags += -lexpat -lcurl
endif

ifeq ($(os),$(filter $(os),Darwin Linux))
lib_srcs += src/core/syshook/unix/iofd.cpp
endif

# library-specific thread support
ifeq ($(display),QT)
include src/display/qt/djnn-lib-flags.mk
lib_srcs += $(shell find src/core/syshook/qt -name "*.cpp")
endif

ifneq (,$(filter $(graphics),CAIRO GL))
include src/display/sdl/djnn-lib-flags.mk
endif

ifeq ($(os),FreeRTOS)
include src/core/syshook/freertos-cxx11/djnn-lib-flags.mk
lib_srcs += $(shell find src/core/syshook/freertos-cxx11 -name "*.cpp")
endif


src/core/djnn-srcs.mk:
	srcs=""
	srcs+=`find src/core -name "*.cpp"`
	echo $$srcs
	echo `find src/core -name "*.cpp"`
	echo lib_srcs := $$srcs > $@

.PHONY: src/core/djnn-srcs.mk
