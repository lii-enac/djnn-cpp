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

lib_djnn_deps = display base core
