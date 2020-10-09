local_dir = $(src_dir)/display
lib_djnn_deps = exec_env core
lib_srcs += $(wildcard $(local_dir)/*.cpp)

ifeq ($(display),DRM)
include src/display/drm/djnn-lib.mk
endif

ifeq ($(display),QT)
include src/display/qt/djnn-lib.mk
lib_srcs += src/display/alt/alt_display.cpp
endif

ifeq ($(display),SDL)
include src/display/sdl/djnn-lib.mk
lib_srcs += src/display/alt/alt_display.cpp
endif

