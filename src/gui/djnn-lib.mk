local_dir = $(src_dir)/gui
lib_djnn_deps = display base exec_env core

lib_srcs += src/gui/abstract_gobj.cpp src/gui/gui.cpp src/gui/layer.cpp src/gui/gui-priv.cpp
submodules = interaction/ picking/ shapes/ style/ transformation/ utils/ widgets/ XML/
lib_srcs += $(call rwildcardmul,$(addprefix $(local_dir)/,$(submodules)),*.cpp)

# ----------
# css parser
lib_srcs += src/gui/css-parser/parser.cpp src/gui/css-parser/scanner.cpp src/gui/css-parser/driver.cpp

# for initial make -j
# find build/src/gui/css-parser -name "*.d" | xargs grep -s "parser.hpp" | awk '{print $1}' | awk -F "." '{print $1".o"}' | sed s/build/\$\(build_dir\)/ | xargs echo

#define my_lib_rules

$(build_dir)/src/gui/css-parser/scanner.o: CXXFLAGS += -Dregister=""
$(build_dir)/src/gui/css-parser/%.o: CXXFLAGS += -I$(build_dir)/src/gui/css-parser -Isrc/gui/css-parser

$(build_dir)/src/gui/css-parser/scanner.o $(build_dir)/src/gui/css-parser/parser.o $(build_dir)/src/gui/css-parser/driver.o: $(build_dir)/src/gui/css-parser/parser.hpp
$(build_dir)/src/gui/css-parser/parser.cpp: src/gui/css-parser/parser.y

ifeq ($(os),MinGW)
# Fix for FlexLexer.h in /usr/include and in /ming64/include
$(build_dir)/src/gui/css-parser/%.o: CXXFLAGS += -I/usr/include
endif
#endef

#lib_rules += $(my_lib_rules)
# ----------

ifeq ($(os),Linux)
YACC = bison -d
endif

ifeq ($(os),Darwin)
lib_cppflags += -I/usr/local/opt/flex/include
lib_ldflags += -L/usr/local/opt/flex/lib
endif

ifeq ($(os),MinGW)
YACC = bison -d
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

