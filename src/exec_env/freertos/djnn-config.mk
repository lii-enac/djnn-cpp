build_dir := build
#display := QT
#graphics := QT
#display := SDL
#graphics := CAIRO

#crazyflie := CRAZYFLIE

#CFLAGS += -DDJNN_NO_DEBUG

#CFLAGS += -fsanitize=thread -O1
#LDFLAGS += -fsanitize=thread

#CFLAGS += -fsanitize=address -O1
#LDFLAGS += -fsanitize=address

#CFLAGS += -fsanitize=memory -O1
#LDFLAGS += -fsanitize=memory

#cross_prefix := em
#em_ext_libs_path := ../djnn-emscripten-ext-libs


djnn_libs := core exec_env base

display :=
graphics :=
os := FreeRTOS
crazyflie_firmware_dir := ../crazyflie-firmware
freertos_dir := $(crazyflie_firmware_dir)/vendor/FreeRTOS
freertos_config_dir := $(crazyflie_firmware_dir)/src/config
freertos_layers_dir := $(crazyflie_firmware_dir)/src
cross_prefix := arm-none-eabi-
thread = STD
CFLAGS += -fno-math-errno -mfp16-format=ieee -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAGS += -fno-math-errno -mfp16-format=ieee -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

