lib_djnn_deps = core
lib_srcs += src/audio/audio.cpp

lib_ldflags += `pkg-config --libs portaudio-2.0`
lib_cppflags += `pkg-config --cflags portaudio-2.0`
