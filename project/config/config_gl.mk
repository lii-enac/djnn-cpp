#verbose build
#V= , 0, 1, 2, 3, max

display := SDL
#options: QT SDL DRM
#options SDL with kmsdrm: ubuntu SDL2 is not compiled with kmsdrm enabled, compile it with the option 
#see how to recompile SDL2 with this option in the file tools/raspberry-ua-netinst.md

graphics := GL
#options: QT CAIRO GL

gl_context := legacy
#gl_context := OSMesa
#gl_context := zink
#gl_context := MGL

picking := COLOR
#option: COLOR ANALYTICAL

audio := AL
#options: AL AL_SOFT

physics := # set to ODE
phidgets := # set to PHIDGETS

use_ivy := yes

djnn_libs_extra :=

# optimization, the higher the level, the faster runtime, the slower compile
# CFLAGS += -O0
# CFLAGS += -O1 # recommended with sanitizer
# CFLAGS += -O2
CFLAGS += -O0

# debug, compile is slower with it
CFLAGS += -g
#CFLAGS += -fstandalone-debug

# sanitizer
#CFLAGS += -fsanitize=address
#LDFLAGS += -fsanitize=address
#CFLAGS += -fsanitize=thread
#LDFLAGS += -fsanitize=thread

# CFLAGS += -Wno-gnu #-null-pointer-arithmetic
#CFLAGS += -fno-limit-debug-info -fstandalone-debug
#LDFLAGS += -fno-limit-debug-info -fstandalone-debug

# Compiler Options Hardening Guide for C and C++
# https://best.openssf.org/Compiler-Hardening-Guides/Compiler-Options-Hardening-Guide-for-C-and-C++.html
CFLAGS += -O2 -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough \
-Werror=format-security \
-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 \
-D_GLIBCXX_ASSERTIONS \
-fstrict-flex-arrays=3 \
-fstack-protector-strong \
#-fstack-clash-protection

LIBS += -Wl,-z,nodlopen -Wl,-z,noexecstack \
-Wl,-z,relro -Wl,-z,now \
-Wl,--as-needed -Wl,--no-copy-dt-needed-entries


build_dir := build

remotery_cflags += -DRMT_ENABLED=1
remotery_cflags += -DRMT_USE_OPENGL=1

keep_intermediate:=yes