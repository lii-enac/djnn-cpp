lib_djnn_deps = exec_env core
local_dir = $(src_dir)/audio
lib_srcs += $(call rwildcard,$(local_dir)/,*.cpp)

ifeq ($(audio),$(filter $(audio),AL AL_SOFT))
lib_cppflags += -DDJNN_USE_OPENAL

	ifeq ($(audio),AL_SOFT)
	lib_cppflags += -DDJNN_USE_OPENAL_EFX
	endif

	#mojoAL
	#lib_srcs += $(call rwildcard,$(local_dir)/,*.c)
	#-I$(local_dir)/al
	#lib_pkg += sdl2

	ifeq ($(os),Darwin)
		ifeq ($(audio),AL)
		# MacOS OpenAL
		lib_ldflags += -framework OpenAL
		endif
		ifeq ($(audio),AL_SOFT)
		# brew openal-soft
		lib_pkgpath += /usr/local/Cellar/openal-soft/1.20.1/lib/pkgconfig
		lib_pkg += openal
		endif
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
endif
#lib_pkg += portaudio-2.0
