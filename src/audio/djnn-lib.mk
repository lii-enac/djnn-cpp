lib_djnn_deps = exec_env core
lib_srcs += $(shell find src/audio -name "*.cpp")
#src/audio/audio.cpp src/audio/sound/sound.cpp src/audio/sound/sample.cpp src/audio/al/al_audio_backend.cpp src/audio/sound/load_wav.cpp

lib_cppflags += -DUSE_OPENAL

ifeq ($(os),Darwin)
#lib_cppflags += -framework OpenAL
lib_ldflags += -framework OpenAL
endif

ifeq ($(os),Linux)
lib_ldflags += -lopenal
endif

ifeq ($(os),em)
lib_ldflags += -lopenal
endif

ifeq ($(os),MinGW)
lib_ldflags += -lopenal
endif

#lib_pkg += portaudio-2.0
