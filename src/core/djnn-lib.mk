local_dir = $(src_dir)/core

lib_srcs += $(local_dir)/core.cpp

submodules = ontology/ control/ property/ tree/ execution/ utils/
ifneq ($(os),$(filter $(os),FreeRTOS crazyflie))
submodules += serializer/ xml/
endif
lib_srcs += $(call rwildcardmul,$(addprefix $(local_dir)/,$(submodules)),*.cpp)

ifeq ($(compiler),llvm)
lib_objs += $(pch_shared_dst)
endif

ifeq ($(remotery_cflags),-DRMT_ENABLED=1)
lib_srcs += $(call rwildcard,$(local_dir)/utils/ext/remotery/,*.c)
$(build_dir)/src/core/utils/ext/remotery/Remotery.o: CFLAGS += -Wno-unused-function -Wno-strict-prototypes -Wno-unused-but-set-variable -Wno-unused-variable -Wno-pedantic
ifeq ($(os),Linux)
lib_ldflags += -lpthread
endif
ifeq ($(os),MinGW)
lib_ldflags += -lwinmm -lWs2_32
endif
endif

ifeq ($(os),Darwin)
lib_pkgpath += $(brew_prefix)/opt/expat/lib/pkgconfig
lib_pkgpath += $(brew_prefix)/opt/curl/lib/pkgconfig
endif

ifneq ($(os),$(filter $(os),FreeRTOS em crazyflie))
lib_pkg += expat libcurl
endif

$(build_dir)/$(local_dir)/xml/XML.o: DJNN_CXXFLAGS += $(shell env PKG_CONFIG_PATH=$(core_lib_pkgpath) pkg-config --cflags expat libcurl)
