local_dir := $(src_dir)/exec_env

lib_cppflags += \
	-DDJNN_USE_FREERTOS=1 \
	-include $(local_dir)/freertos-cxx11/freertos-cxx11-macros.h \
	-I$(local_dir)/freertos-cxx11 \
	-I$(freertos_dir)/include \
	-I$(freertos_dir)/portable/GCC/ARM_CM4F \
	-I$(freertos_config_dir) \
	-I$(freertos_layers_dir)/drivers/interface \
	-I$(freertos_layers_dir)/hal/interface \
	-I$(freertos_layers_dir)/utils/interface \
	-I$(freertos_layers_dir)/modules/interface
