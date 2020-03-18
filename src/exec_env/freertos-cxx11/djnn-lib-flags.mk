lib_cppflags += \
	-include src/exec_env/freertos-cxx11/freertos-cxx11-macros.h \
	-Isrc/exec_env/freertos-cxx11 \
	-I$(freertos_dir)/include -I$(freertos_dir)/portable/GCC/ARM_CM4F -I$(freertos_config_dir) \
	-I../crazyflie-firmware/src/drivers/interface -I../crazyflie-firmware/src/hal/interface -I../crazyflie-firmware/src/utils/interface \
	-I../crazyflie-firmware/src/modules/interface  \
	-DDJNN_USE_FREERTOS=1