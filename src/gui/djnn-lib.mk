local_dir = $(src_dir)/gui
lib_djnn_deps = display base exec_env core

lib_srcs += src/gui/abstract_gobj.cpp src/gui/gui.cpp src/gui/layer.cpp src/gui/gui-priv.cpp
submodules = interaction/ picking/ shape/ style/ transformation/ utils/ widgets/ XML/
lib_srcs += $(call rwildcardmul,$(addprefix $(local_dir)/,$(submodules)),*.cpp)

# ----------
# css parser
lib_srcs += src/gui/css-parser/parser.cpp src/gui/css-parser/scanner.cpp src/gui/css-parser/driver.cpp

# for initial make -j
# find build/src/gui/css-parser -name "*.d" | xargs grep -s "parser.hpp" | awk '{print $1}' | awk -F "." '{print $1".o"}' | sed s/build/\$\(build_dir\)/ | xargs echo

#define my_lib_rules

$(build_dir)/src/gui/css-parser/scanner.o: DJNN_CXXFLAGS += -Wno-implicit-fallthrough
$(build_dir)/src/gui/css-parser/%.o: DJNN_CXXFLAGS += -I$(build_dir)/src/gui/css-parser -Isrc/gui/css-parser -Wno-misleading-indentation

$(build_dir)/src/gui/css-parser/scanner.o $(build_dir)/src/gui/css-parser/parser.o $(build_dir)/src/gui/css-parser/driver.o: $(build_dir)/src/gui/css-parser/parser.hpp
$(build_dir)/src/gui/css-parser/parser.cpp: src/gui/css-parser/parser.y
$(build_dir)/src/gui/css-parser/parser.hpp: src/gui/css-parser/parser.y
$(build_dir)/src/gui/css-parser/location.hh: src/gui/css-parser/parser.y


ifeq ($(os),MinGW)
# Fix for FlexLexer.h in /usr/include and in /ming64/include
$(build_dir)/src/gui/css-parser/%.o: DJNN_CXXFLAGS += -I/usr/include
endif
#endef

ifeq ($(os),Darwin)
$(build_dir)/src/gui/css-parser/%.o: DJNN_CXXFLAGS += -I$(brew_prefix)/opt/flex/include -Wno-unused-but-set-variable
lib_ldflags += -L$(brew_prefix)/opt/flex/lib
endif

ifeq ($(os),Browser)
brew_prefix := $(shell brew --prefix)
$(build_dir)/src/gui/css-parser/%.o: DJNN_CXXFLAGS += -I$(brew_prefix)/opt/flex/include -Wno-unused-but-set-variable
lib_ldflags += -L$(brew_prefix)/opt/flex/lib
endif


ifeq ($(picking),ANALYTICAL)
lib_cppflags += -DDJNN_USE_ANALYTICAL_PICKING
else ifeq ($(picking),COLOR)
lib_cppflags += -DDJNN_USE_COLOR_PICKING
else ifeq ($(picking),)
lib_cppflags += -DDJNN_USE_COLOR_PICKING
else
$(warning("unkown picking method"))
endif


ifeq ($(graphics),QT)
include src/gui/qt/djnn-lib.mk
endif

ifeq ($(graphics),CAIRO)
include src/gui/cairo/djnn-lib.mk
endif

ifeq ($(graphics),GL)
include src/gui/gl/djnn-lib.mk
endif

ifeq ($(graphics),EGL)
include src/gui/egl/djnn-lib.mk
endif


# ifneq ($(os),$(filter $(os),FreeRTOS em crazyflie))
# lib_pkg += fmt
# endif
