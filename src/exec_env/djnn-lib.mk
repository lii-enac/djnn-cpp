lib_djnn_deps = core
local_dir = $(src_dir)/exec_env
lib_srcs += $(wildcard $(local_dir)/*.cpp)

ifeq ($(os),Linux)
lib_suffix =.so
boost_libs = -lboost_thread -lboost_chrono -lboost_system
#-lboost_fiber-mt -lboost_context-mt
DYNLIB = -shared
endif

ifeq ($(os),Darwin)
lib_suffix =.dylib
# for gcc
lib_ldflags += -L/usr/local/lib/
boost_libs = -lboost_thread-mt -lboost_chrono-mt -lboost_system-mt -lboost_fiber-mt -lboost_context-mt
endif

ifeq ($(os),MinGW)
lib_suffix =.dll
boost_libs = -lboost_thread-mt -lboost_chrono-mt -lboost_system-mt
lib_ldflags += -lws2_32
endif

ifeq ($(os),crazyflie)
# crazyflie
lib_cflags += -I$(freertos_dir)/portable/GCC/ARM_CM4F
lib_cflags += -DSTM32F40_41xxx
lib_cflags += -I$(crazyflie_firmware_dir)/src/lib/CMSIS/STM32F4xx/Include
lib_cflags += -I$(crazyflie_firmware_dir)/vendor/CMSIS/CMSIS/Include/
#lib_cflags += -mfp16-format=ieee -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
lib_cflags += -fexceptions
#lib_cflags += -ffunction-sections -fdata-sections
lib_cflags += -fno-math-errno -fno-strict-aliasing -Wdouble-promotion
#lib_cflags += -fPIC
#lib_cflags += -pie
endif


ifeq ($(display),QT)

ifeq ($(os),Darwin)
thread ?= STD
chrono ?= STD
else
thread ?= QT
chrono ?= STD
endif

else ifeq ($(display),SDL)
thread ?= SDL
chrono ?= STD

else ifeq ($(display),DRM)
thread ?= STD
chrono ?= STD

else ifeq ($(display),)
thread ?= STD
chrono ?= STD

else
$(warning "unknown display (choose among: QT, SDL, DRM, (none))")

endif

thread ?= BOOST
#options: QT SDL STD FIBER
chrono ?= STD
#options: QT FIBER STD

ifeq ($(thread),BOOST)
lib_cppflags += -DDJNN_USE_BOOST_THREAD=1
lib_ldflags += $(boost_libs)

else ifeq ($(thread),QT)
lib_cppflags += -DDJNN_USE_QT_THREAD=1

else ifeq ($(thread),SDL)
lib_cppflags += -DDJNN_USE_SDL_THREAD=1

else ifeq ($(thread),STD)
lib_cppflags += -DDJNN_USE_STD_THREAD=1

endif


ifeq ($(chrono),BOOST)
lib_cppflags += -DDJNN_USE_BOOST_CHRONO=1
ifneq ($(thread),BOOST)
lib_ldflags += $(boost_libs)
endif

else ifeq ($(chrono),STD)
lib_cppflags += -DDJNN_USE_STD_CHRONO=1

else ifeq ($(chrono),SDL)
lib_cppflags += -DDJNN_USE_SDL_CHRONO=1
endif



ifeq ($(compiler),gnu)
	lib_ldflags += -latomic
endif

ifeq ($(os),$(filter $(os),Darwin Linux MinGW))
lib_srcs += src/exec_env/unix/iofd.cpp
endif

ifeq ($(display),QT)
include src/display/qt/djnn-lib-flags.mk
lib_srcs += $(call rwildcard,src/exec_env/qt/,*.cpp)
$(build_dir)/$(local_dir)/main_loop.o \
$(build_dir)/$(local_dir)/qt/qt_mainloop.o: \
DJNN_CXXFLAGS += -DDJNN_USE_QT_MAINLOOP=1 $(shell env PKG_CONFIG_PATH=$(exec_env_lib_pkgpath) pkg-config --cflags Qt5OpenGL Qt5Gui)
endif

ifeq ($(display),SDL)
include src/display/sdl/djnn-lib-flags.mk
lib_srcs += $(call rwildcard,src/exec_env/sdl/,*.cpp)
endif

ifeq ($(os),crazyflie)
include src/exec_env/crazyflie/djnn-lib.mk
endif
