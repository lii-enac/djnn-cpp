lib_djnn_deps = display base exec_env core

lib_srcs += src/gui/abstract_gobj.cpp src/gui/gui.cpp
lib_srcs += $(shell find src/gui/interaction -name "*.cpp")
lib_srcs += $(shell find src/gui/picking -name "*.cpp")
lib_srcs += $(shell find src/gui/shapes -name "*.cpp")
lib_srcs += $(shell find src/gui/style -name "*.cpp")
lib_srcs += $(shell find src/gui/transformation -name "*.cpp")
lib_srcs += $(shell find src/gui/XML -name "*.cpp")
lib_srcs += $(shell find src/gui/widgets -name "*.cpp")
lib_srcs += src/gui/css-parser/parser.cpp src/gui/css-parser/scanner.cpp src/gui/css-parser/driver.cpp 

$(build_dir)/src/gui/css-parser/scanner.o: CXXFLAGS += -Dregister=""
#$(build_dir)/src/gui/css-parser/%.o: $(build_dir)/src/gui/css-parser/parser.hpp
$(build_dir)/src/gui/css-parser/%.o: CXXFLAGS += -I$(build_dir)/src/gui/css-parser -Isrc/gui/css-parser

# for initial make -j
# find build/src/gui/css-parser -name "*.d" | xargs grep -s "parser.hpp" | awk '{print $1}' | awk -F "." '{print $1".o"}' | sed s/build/\$\(build_dir\)/ | xargs echo
#$(build_dir)/src/gui/css-parser/scanner.o $(build_dir)/src/gui/css-parser/parser.o $(build_dir)/src/gui/css-parser/driver.o: $(build_dir)/src/gui/css-parser/parser.hpp

ifeq ($(os),Linux)
YACC = bison -d
endif

ifeq ($(os),Darwin)
YACC := /usr/local/opt/bison/bin/bison -d
LEX := /usr/local/opt/flex/bin/flex
lib_cppflags += -I/usr/local/opt/flex/include
lib_ldflags += -L/usr/local/opt/flex/lib
endif

ifeq ($(os),MinGW)
CXXFLAGS += -I/usr/include # Fix for FlexLexer.h in /usr/include and in /ming64/include
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

