local_dir = $(src_dir)/core

lib_srcs += $(local_dir)/core.cpp

submodules = ontology/ control/ tree/ execution/ serializer/ utils/
lib_srcs += $(call rwildcardmul,$(addprefix $(local_dir)/,$(submodules)),*.cpp)

ifneq ($(os),FreeRTOS)
lib_srcs += $(call rwildcard,$(local_dir)/xml/,*.cpp)
endif

#lib_srcs += $(call rwildcard,$(local_dir)/utils/ext/remotery/,*.c)
#lib_cflags += -DRMT_ENABLED=1 -DRMT_USE_OPENGL=0
CFLAGS += -DRMT_ENABLED=0 # remotery - get rid of any call

ifeq ($(os),Darwin)
lib_pkgpath += /usr/local/opt/expat/lib/pkgconfig
lib_pkgpath += /usr/local/opt/curl/lib/pkgconfig
endif

ifneq ($(os),$(filter $(os),FreeRTOS em))
#lib_ldflags += -lexpat -lcurl
lib_pkg += expat libcurl
endif
