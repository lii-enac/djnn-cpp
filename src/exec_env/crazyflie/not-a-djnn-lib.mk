# -- crazyflie/freertos

# not-a-djnn-lib because it doesn't follow the conventions of djnn modules
# this is a WIP, don't run this on your flying taxi-drone

# to use it:
#
# in djnn-cpp/ :
#	make -j static
# in crazyflie-firmware/ :
#	make -j V=1 djnn=1 && make cload

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

CFLAGS += -DDJNN_NO_DEBUG
CFLAGS += -DDJNN_NO_SERIALIZE

CFLAGS += -fno-math-errno -mfp16-format=ieee -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAGS += -fno-math-errno -mfp16-format=ieee -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

# IntProperty
crazyflie_objs := \
	src/exec_env/crazyflie/crazyflie.o \
	src/core/core.o src/core/ontology/process.o src/core/ontology/coupling.o src/core/execution/graph.o \
	src/core/utils/error.o src/core/utils/uri.o \
	src/core/tree/int_property.o \
# graph: for state_dependency
# uri: for find_by_uri

# Binding, nothing to add!
crazyflie_objs +=

# Assignment
# crazyflie_objs += \
# 	src/core/control/action.o src/core/control/simple_assignment.o \
# 	src/core/tree/bool_property.o src/core/tree/double_property.o src/core/tree/text_property.o src/core/tree/ref_property.o \
# 	src/core/tree/spike.o
# spike : for bool

.PHONY: $(build_dir)/include/djnn/crazyflie.h $(build_dir)/include/djnn/crazyflie-dev.h

