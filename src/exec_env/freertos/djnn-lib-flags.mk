local_dir := $(src_dir)/exec_env/freertos

# freertos
freertos_flags += \
	-D_GCC_MULTITHREAD_FREERTOS_ENABLE \
	-I$(freertos_dir)/include \
	-I$(freertos_config_dir) \
	-I$(freertos_layers_dir)/drivers/interface \
	-I$(freertos_layers_dir)/hal/interface \
	-I$(freertos_layers_dir)/utils/interface \
	-I$(freertos_layers_dir)/modules/interface

lib_cflags += $(freertos_flags)

#freetos-cxx11
lib_cppflags += \
	-include $(local_dir)/ext/freertos-cxx11/freertos-cxx11-macros.h \
	-I$(local_dir)/ext/freertos-cxx11 \

#-I$(freertos_dir)/portable/GCC/ARM_CM4F \
