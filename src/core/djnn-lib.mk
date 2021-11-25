local_dir = $(src_dir)/core

lib_srcs += $(local_dir)/core.cpp

submodules = ontology/ control/ tree/ execution/ utils/
ifneq ($(os),$(filter $(os),FreeRTOS crazyflie))
submodules += serializer/ xml/
endif
lib_srcs += $(call rwildcardmul,$(addprefix $(local_dir)/,$(submodules)),*.cpp)

ifeq ($(remotery_cflags),-DRMT_ENABLED=1)
lib_srcs += $(call rwildcard,$(local_dir)/utils/ext/remotery/,*.c)
ifeq ($(os),MinGW)
lib_ldflags += -lwinmm -lWs2_32
endif
endif

ifeq ($(os),Darwin)
lib_pkgpath += $(shell brew --prefix expat)/lib/pkgconfig
lib_pkgpath += $(shell brew --prefix curl)/lib/pkgconfig
endif

ifneq ($(os),$(filter $(os),FreeRTOS em crazyflie))
lib_pkg += expat libcurl # fmt
endif
