lib_cppflags += \
	-include src/core/exec_env/freertos-cxx11/freertos-cxx11-macros.h \
	-Isrc/core/exec_env/freertos-cxx11 \
	-I$(freertos_dir)/include -I$(freertos_config_dir) \
	-I../crazyflie-firmware/src/drivers/interface -I../crazyflie-firmware/src/hal/interface -I../crazyflie-firmware/src/utils/interface \
	-I../crazyflie-firmware/src/modules/interface -I../crazyflie-firmware/src/lib/FreeRTOS/portable/GCC/ARM_CM4F \
	-DDJNN_USE_FREERTOS=1