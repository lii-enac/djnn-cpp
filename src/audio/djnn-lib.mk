lib_djnn_deps = core
lib_srcs += $(shell find src/audio -name "*.cpp")
#src/audio/audio.cpp src/audio/sound/sound.cpp src/audio/sound/sample.cpp src/audio/al/al_audio_backend.cpp src/audio/sound/load_wav.cpp

lib_cppflags += -DUSE_OPENAL

ifeq ($(os),Darwin)
lib_cppflags += -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/OpenAL.framework/Headers
lib_ldflags += -framework OpenAL
endif

ifeq ($(os),em)
lib_ldflags += -lopenal
endif

ifeq ($(os),MinGW)
lib_ldflags += -lopenal
endif


#lib_ldflags += `pkg-config --libs portaudio-2.0`
#lib_cppflags += `pkg-config --cflags portaudio-2.0`
