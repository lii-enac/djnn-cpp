local_dir := $(src_dir)/exec_env/freertos

lib_cppflags += \
	-D_GCC_MULTITHREAD_FREERTOS_ENABLE \
	-include $(local_dir)/ext/freertos-cxx11/freertos-cxx11-macros.h \
	-I$(local_dir)/ext/freertos-cxx11 \
	-I$(freertos_dir)/include \
	-I$(freertos_dir)/portable/GCC/ARM_CM4F \
	-I$(freertos_config_dir) \
	-I$(freertos_layers_dir)/drivers/interface \
	-I$(freertos_layers_dir)/hal/interface \
	-I$(freertos_layers_dir)/utils/interface \
	-I$(freertos_layers_dir)/modules/interface
