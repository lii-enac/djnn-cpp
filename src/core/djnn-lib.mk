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

ifneq (-DRMT_ENABLED=0,$(filter -DRMT_ENABLED=0,$(remotery_cflags)))
lib_srcs += $(call rwildcard,$(local_dir)/utils/ext/remotery/,*.c)
$(build_dir)/src/core/utils/ext/remotery/Remotery.o: CFLAGS += -Wno-unused-function -Wno-strict-prototypes -Wno-unused-but-set-variable -Wno-unused-variable -Wno-pedantic
ifeq (-DRMT_USE_OPENGL=1,$(filter -DRMT_USE_OPENGL=1,$(remotery_cflags)))
#lib_srcs += src/gui/gl/gl2d/ext/glad/src/glad.c # does not work since it creates another glad set of objects
$(build_dir)/src/core/utils/ext/remotery/Remotery.o: CFLAGS += -Isrc/gui/gl/gl2d -Isrc/gui/gl/gl2d/ext/glad/include
prefix := -Wl,-U,_glad_gl
glad_weak_functions := $(addprefix $(prefix),GetError GenQueries DeleteQueries BeginQuery EndQuery GetQueryObjectiv GetQueryObjectuiv GetQueryObjecti64v GetQueryObjectui64v QueryCounter GetInteger64v Finish)
ifeq ($(linker),llvm)
lib_ldflags += $(glad_weak_functions)
endif
#$(info echo $(glad_weak_functions))
endif
ifeq ($(os),Linux)
lib_ldflags += -lpthread
endif
ifeq ($(os),MinGW)
lib_ldflags += -lwinmm -lWs2_32
endif
endif

#ifeq ($(os),Darwin)
#lib_pkgpath += $(brew_prefix)/opt/expat/lib/pkgconfig
#lib_pkgpath += $(brew_prefix)/opt/curl/lib/pkgconfig
#endif

use_expat ?= yes
ifeq ($(use_expat),yes)
lib_pkg += expat
lib_cppflags += -DDJNN_USE_EXPAT=1
endif

use_curl ?= yes
ifeq ($(use_curl),yes)
lib_pkg += libcurl
lib_cppflags += -DDJNN_USE_CURL=1
endif

$(build_dir)/$(local_dir)/xml/XML.o: DJNN_CXXFLAGS += $(shell env PKG_CONFIG_PATH=$(core_lib_pkgpath) pkg-config --cflags expat libcurl)
