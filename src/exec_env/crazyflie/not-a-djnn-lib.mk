# -- crazyflie/freertos

# not-a-djnn-lib because it doesn't follow the conventions of djnn modules
# this is a WIP, don't run this on your flying taxi-drone

# to use it:
# in djnn-cpp/ :
#	make -j static
# in crazyflie-firmware/ :
#	make -j V=1 djnn=1 && make cload
# in cfclient :
# 	menu View > Tabs > Console
# 	press 'clear' and 'connect'

# -------------------------------------

src_dir ?= src

display :=
graphics :=
os := FreeRTOS

djnn_libs := crazyflie
cross_prefix := arm-none-eabi-
thread = STD

# freertos
freertos_dir := $(crazyflie_firmware_dir)/vendor/FreeRTOS
freertos_config_dir := $(crazyflie_firmware_dir)/src/config
freertos_layers_dir := $(crazyflie_firmware_dir)/src

include src/exec_env/freertos/djnn-lib-flags.mk

# crazyflie
lib_cflags += -I$(freertos_dir)/portable/GCC/ARM_CM4F
lib_cflags += -DSTM32F40_41xxx
lib_cflags += -I$(crazyflie_firmware_dir)/src/lib/CMSIS/STM32F4xx/Include
lib_cflags += -I$(crazyflie_firmware_dir)/vendor/CMSIS/CMSIS/Include/
lib_cflags += -mfp16-format=ieee -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
lib_cflags += -fexceptions
lib_cflags += -ffunction-sections -fdata-sections
lib_cflags += -fno-math-errno -fno-strict-aliasing -Wdouble-promotion
#lib_cflags += -fPIC
#lib_cflags += -pie

#boost name demangle
lib_cppflags += -I/usr/local/include

# djnn
lib_cflags += -DDJNN_CRAZYFLIE
lib_cflags += -DDJNN_NO_DEBUG
lib_cflags += -DDJNN_NO_SERIALIZE
lib_cflags += -DRMT_ENABLED=0
lib_cppflags += -DDJNN_USE_FREERTOS
lib_cppflags += -DDJNN_USE_STD_THREAD=1
lib_cppflags += --rtti #--rtti_data
lib_cppflags += -Wno-psabi #https://stackoverflow.com/a/48149400
#lib_cppflags += -DDJNN_NO_DYNAMIC_CAST
#lib_cppflags += -DDJNN_USE_FREERTOS_MAINLOOP

#LDFLAGS += -z now -z relro

# it's not a djnn lib .mk !!
CFLAGS := $(lib_cflags)
CXXFLAGS := $(lib_cppflags)

toto:
	@echo $(CXXFLAGS) | tr " " "\n" | sort ;
	#| tr "\n" " "; echo 

crazyflie_objs += \
	src/core/core.o

# Properties
crazyflie_objs += \
	src/core/ontology/process.o src/core/ontology/coupling.o src/core/execution/graph.o \
	src/core/tree/int_property.o src/core/tree/bool_property.o src/core/tree/text_property.o \
	src/core/tree/double_property.o src/core/tree/ref_property.o \
	src/core/utils/error.o src/core/utils/uri.o src/core/utils/utils-dev.o
# graph: for state_dependency
# uri: for find_by_uri

# Binding
crazyflie_objs += src/core/control/binding.o

# Assignment
crazyflie_objs += \
	src/core/control/action.o src/core/control/assignment.o \
	src/core/tree/bool_property.o src/core/tree/double_property.o src/core/tree/text_property.o src/core/tree/ref_property.o \
	src/core/tree/spike.o \
	src/core/tree/blank.o
# spike: for bool
# blank: for time

# Component
crazyflie_objs += \
	src/core/tree/component.o \
	src/core/tree/component_observer.o

crazyflie_objs += \
	src/core/control/native_action.o \
	src/core/control/native_expression_action.o \
	src/core/control/synchronizer.o \
	src/core/control/activator.o


# base
crazyflie_objs += \
	src/base/base.o

crazyflie_objs += \
	src/base/connector.o \
	src/base/fsm.o

crazyflie_objs += \
	src/base/text.o

# exec_env
crazyflie_objs += \
	src/exec_env/exec_env.o

crazyflie_objs += \
	src/exec_env/djnn_time_manager.o \
	src/exec_env/exit.o \
	src/exec_env/external_source.o \
	src/exec_env/global_mutex.o \
	src/exec_env/main_loop.o \
	src/exec_env/time_manager.o \
	src/exec_env/timer.o \
	src/exec_env/freertos/freertos_mainloop.o \
	src/exec_env/freertos/ext/freertos-cxx11/condition_variable.o \
	src/exec_env/freertos/ext/freertos-cxx11/freertos_time.o \
	src/exec_env/freertos/ext/freertos-cxx11/future.o \
	src/exec_env/freertos/ext/freertos-cxx11/gthr_key.o \
	src/exec_env/freertos/ext/freertos-cxx11/mutex.o \
	src/exec_env/freertos/ext/freertos-cxx11/thread.o \
	src/exec_env/freertos/memory.o
#	src/exec_env/freertos/freertos_mainloop.o # not used as of yet


.PHONY: $(build_dir)/include/djnn/crazyflie.h $(build_dir)/include/djnn/crazyflie-dev.h
