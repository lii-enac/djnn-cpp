display := QT
#options: QT SDL

graphics := QT
#options: QT CAIRO

physics := # set to ODE
phidgets := # set to PHIDGETS

# cross-compile support
cross_prefix := g
#options: g llvm-g arm-none-eabi- em
#options: /usr/local/Cellar/android-ndk/r14/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-g
#options: /Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-c

# emscripten ext libs
#em_ext_libs_path := ../djnn-emscripten-ext-libs

build_dir := build

#picking := COLOR
#option: color analytical

#CFLAGS += -DDJNN_NO_DEBUG
