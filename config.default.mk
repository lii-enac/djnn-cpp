#verbose build
#V= , 0, 1, 2, 3, max

display := QT
#options: QT SDL DRM
#options SDL with kmsdrm: ubuntu SDL2 is not compiled with kmsdrm enabled, compile it with the option 
#see how to recompile SDL2 with this option in the file tools/raspberry-ua-netinst.md

graphics := QT
#options: QT CAIRO

picking := COLOR
#option: COLOR ANALYTICAL

audio := AL
#options: AL AL_SOFT

physics := # set to ODE
phidgets := # set to PHIDGETS

use_ivy := no

djnn_libs_extra :=

# very fast compilation, but no debug
# CFLAGS += -O0
# fast compilation and debug
# CFLAGS += -O0 -g
# optimized but slower compilation
# CFLAGS += -O3
# optimized and debug but even slower compilation
# CFLAGS += -O3 -g

# cross-compile support
#cross_prefix :=
#options: g llvm-g arm-none-eabi- em
#options: /usr/local/Cellar/android-ndk/r14/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-g
#options: /Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-c

# emscripten
# in the shell: source /Users/conversy/recherche/istar/code/apps/ext-libs/emscripten/emsdk/emsdk_env.sh
# cross_prefix := em
# CXX := ++
# CXXFLAGS += -I/usr/local/Cellar/flex/2.6.4_1/include/
# CXXFLAGS += -DGL2D_RPI2 -DMASK_STB
# display := SDL
# graphics := GL
# picking := ANALYTICAL
# audio := AL


build_dir := build


#CFLAGS += -DDJNN_NO_DEBUG

# for clang-tidy
#CFLAGS += -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1
#CFLAGS += -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include

remotery_cflags?=-DRMT_ENABLED=0
