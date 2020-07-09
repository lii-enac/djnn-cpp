# -- crazyflie/freertos

# not-a-djnn-lib because it doesn't follow the conventions of djnn modules
# this is a WIP, don't run this on your flying taxi-drone

# to use it:
#
# in djnn-cpp/ :
#	make -j static
# in crazyflie-firmware/ :
#	make -j V=1 djnn=1 && make cload
# in cfclient :
# 	menu View > Tabs > Console
# 	press 'clear' and 'connect'

# -------------------------------------

display :=
graphics :=
os := FreeRTOS

djnn_libs := crazyflie

freertos_dir := $(crazyflie_firmware_dir)/vendor/FreeRTOS
freertos_config_dir := $(crazyflie_firmware_dir)/src/config
freertos_layers_dir := $(crazyflie_firmware_dir)/src
cross_prefix := arm-none-eabi-
thread = STD

CFLAGS += -DDJNN_CRAZYFLIE
CFLAGS += -DDJNN_NO_DEBUG
CFLAGS += -DDJNN_NO_SERIALIZE
 
CFLAGS += -mfp16-format=ieee -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
#CFLAGS += -pie
CFLAGS += -fexceptions
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -fno-math-errno -fno-strict-aliasing -Wdouble-promotion
#CFLAGS += -fpic

#CXXFLAGS += $(CFLAGS)
#CXXFLAGS += -DDJNN_NO_DYNAMIC_CAST
CXXFLAGS += --rtti #--rtti_data
CXXFLAGS += -Wno-psabi #https://stackoverflow.com/a/48149400
# CXXFLAGS += -I$(freertos_dir)/include \
# 	-I$(freertos_dir)/portable/GCC/ARM_CM4F \
# 	-I$(freertos_config_dir) \
# 	-I$(freertos_layers_dir)/drivers/interface \
# 	-I$(freertos_layers_dir)/hal/interface \
# 	-I$(freertos_layers_dir)/utils/interface \
# 	-I$(freertos_layers_dir)/modules/interface
#boost
CXXFLAGS += -I/usr/local/include

#CXXFLAGS += -fno-rtti

#LDFLAGS += -fno-math-errno -mfp16-format=ieee -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
#LDFLAGS += -z now -z relro

#include src/exec_env/freertos/djnn-lib-flags.mk
#CFLAGS += $(CFLAGS)
# CFLAGS += -I$(freertos_layers_dir) -I$(freertos_config_dir)
# CFLAGS += \
# 	-D_GCC_MULTITHREAD_FREERTOS_ENABLE \
# 	-include $(src_dir)/exec_env/freertos/ext/freertos-cxx11/freertos-cxx11-macros.h \
# 	-I$(src_dir)/exec_env/freertos/ext/freertos-cxx11 \
# 	-I$(freertos_dir)/include \
# 	-I$(freertos_dir)/portable/GCC/ARM_CM4F \
# 	-I$(freertos_config_dir) \
# 	-I$(freertos_layers_dir)/drivers/interface \
# 	-I$(freertos_layers_dir)/hal/interface \
# 	-I$(freertos_layers_dir)/utils/interface \
# 	-I$(freertos_layers_dir)/modules/interface


toto:
	@echo $(CXXFLAGS) | tr " " "\n" | sort ;
	#| tr "\n" " "; echo 

# minimum
# crazyflie_objs := \
# 	src/exec_env/crazyflie/crazyflie.o \
# 	src/core/core.o \

crazyflie_objs += \
	src/core/core.o

# IntProperty
crazyflie_objs += \
	src/core/ontology/process.o src/core/ontology/coupling.o src/core/execution/graph.o \
	src/core/utils/error.o src/core/utils/uri.o src/core/utils/utils-dev.o \
	src/core/tree/int_property.o
# graph: for state_dependency
# uri: for find_by_uri

# Binding, nothing to add!
crazyflie_objs +=

# Assignment
crazyflie_objs += \
	src/core/control/action.o src/core/control/assignment.o \
	src/core/tree/bool_property.o src/core/tree/double_property.o src/core/tree/text_property.o src/core/tree/ref_property.o \
	src/core/tree/spike.o
# spike : for bool

crazyflie_objs += \
	src/base/base.o

crazyflie_objs += \
	src/base/connector.o \
	src/base/fsm.o


.PHONY: $(build_dir)/include/djnn/crazyflie.h $(build_dir)/include/djnn/crazyflie-dev.h

