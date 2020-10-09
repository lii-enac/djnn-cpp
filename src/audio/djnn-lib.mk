lib_djnn_deps = exec_env core
local_dir = $(src_dir)/audio
lib_srcs += $(call rwildcard,$(local_dir)/,*.cpp)

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
