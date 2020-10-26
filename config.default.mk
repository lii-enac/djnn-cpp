#verbose build
#V= , 0, 1, 2, 3, max

display := QT
#options: QT SDL DRM

graphics := QT
#options: QT CAIRO

audio := AL
#options: AL AL_SOFT

physics := # set to ODE
phidgets := # set to PHIDGETS

djnn_libs_extra :=


# cross-compile support
#cross_prefix :=
#options: g llvm-g arm-none-eabi- em
#options: /usr/local/Cellar/android-ndk/r14/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-g
#options: /Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-c

# emscripten ext libs
#em_ext_libs_path := ../djnn-emscripten-ext-libs

build_dir := build

picking := COLOR
#option: color analytical

#CFLAGS += -DDJNN_NO_DEBUG
