lib_djnn_deps = exec_env core
local_dir = $(src_dir)/audio
lib_srcs += $(call rwildcard,$(local_dir)/,*.cpp)
lib_srcs += $(local_dir)/ext/dr_impl.c

ifeq ($(os),Darwin)
lib_cppflags += -Wno-deprecated-declarations
endif

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
		lib_ldflags += -F/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks
		lib_ldflags += -framework OpenAL
		endif
		ifeq ($(audio),AL_SOFT)
		# brew openal-soft
		lib_pkgpath += $(brew_prefix)/opt/openal-soft/lib/pkgconfig
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

# ifneq ($(os),$(filter $(os),FreeRTOS em crazyflie))
# lib_pkg += fmt
# endif
