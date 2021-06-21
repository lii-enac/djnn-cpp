local_dir = $(src_dir)/core

lib_srcs += $(local_dir)/core.cpp

submodules = ontology/ control/ tree/ execution/ utils/
ifneq ($(os),$(filter $(os),FreeRTOS crazyflie))
submodules += serializer/ xml/
endif
lib_srcs += $(call rwildcardmul,$(addprefix $(local_dir)/,$(submodules)),*.cpp)

ifneq ($(remotery_cflags),-DRMT_ENABLED=0)
lib_srcs += $(call rwildcard,$(local_dir)/utils/ext/remotery/,*.c)
endif

ifeq ($(os),Darwin)
lib_pkgpath += /usr/local/opt/expat/lib/pkgconfig
lib_pkgpath += /usr/local/opt/curl/lib/pkgconfig
endif

ifneq ($(os),$(filter $(os),FreeRTOS em crazyflie))
lib_pkg += expat libcurl # fmt
endif
