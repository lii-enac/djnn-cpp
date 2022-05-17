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

lib_srcs += src/exec_env/freertos/freertos_mainloop.cpp \
	src/exec_env/freertos/ext/freertos-cxx11/condition_variable.cpp \
	src/exec_env/freertos/ext/freertos-cxx11/freertos_time.cpp \
	src/exec_env/freertos/ext/freertos-cxx11/future.cpp \
	src/exec_env/freertos/ext/freertos-cxx11/gthr_key.cpp \
	src/exec_env/freertos/ext/freertos-cxx11/mutex.cpp \
	src/exec_env/freertos/ext/freertos-cxx11/thread.cpp \
	src/exec_env/freertos/memory.cpp
