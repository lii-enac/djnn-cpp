lib_djnn_deps = core

lib_srcs += src/exec_env/external_source.cpp src/exec_env/exec_env.cpp \
			src/exec_env/global_mutex.cpp src/exec_env/main_loop.cpp src/exec_env/exit.cpp \
			src/exec_env/timer.cpp src/exec_env/time_manager.cpp src/exec_env/djnn_time_manager.cpp

lib_headers += $(shell find src/exec_env -name "*.h")


ifeq ($(os),Linux)
lib_suffix =.so
boost_libs = -lboost_thread -lboost_chrono -lboost_system
#-lboost_fiber-mt -lboost_context-mt
DYNLIB = -shared
lib_cflags += -fpic -g -MMD -Wall
endif

ifeq ($(os),Darwin)
lib_suffix =.dylib
boost_libs = -lboost_thread-mt -lboost_chrono-mt -lboost_system-mt -lboost_fiber-mt -lboost_context-mt
endif

ifeq ($(os),MinGW)
lib_suffix =.dll
boost_libs = -lboost_thread-mt -lboost_chrono-mt -lboost_system-mt
lib_cppflags += -I/usr/include # Fix for FlexLexer.h in /usr/include and in /ming64/include
endif

ifeq ($(os),FreeRTOS)
boost_libs =
endif


ifeq ($(display),QT)

ifeq ($(os),Darwin)
thread ?= BOOST
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
ifeq ($(os),Darwin)
lib_cppflags += -I/usr/local/include
endif

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
ifeq ($(os),Darwin)
lib_cppflags += -I/usr/local/include
endif
lib_ldflags += $(boost_libs)
endif

else ifeq ($(chrono),STD)
lib_cppflags += -DDJNN_USE_STD_CHRONO=1

else ifeq ($(chrono),SDL)
lib_cppflags += -DDJNN_USE_SDL_CHRONO=1
endif



ifeq (g++,$(findstring g++,$(CXX)))
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
lib_srcs += $(shell find src/exec_env/sdl -name "*.cpp")
endif

ifeq ($(os),FreeRTOS)
include src/exec_env/freertos/djnn-lib-flags.mk
lib_cppflags += -DDJNN_USE_FREERTOS_MAINLOOP=1
lib_srcs += src/exec_env/freertos/freertos_mainloop.cpp
lib_srcs += src/exec_env/crazyflie/crazyflie.cpp
lib_srcs += $(shell find src/exec_env/freertos/ext/freertos-cxx11 -name "*.cpp")
endif
