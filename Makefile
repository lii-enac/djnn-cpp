default: all
.PHONY: default

all: config.mk djnn
.PHONY: all

help:
	@echo "default: djnn ; all: djnn"
	@echo "experiment make -j !!"

config.mk:
	cp config.mk.default config.mk
include config.mk.default
-include config.mk

# remove builtin rules: speed up build process and help debug
MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

# cross-platform support
crossp :=
#crossp := g
#crossp := arm-none-eabi-
#crossp := i686-w64-mingw32-
#brew install mingw-w64, git clone https://github.com/meganz/mingw-std-threads, https://bitbucket.org/skunkos/qt5-minimalistic-builds/downloads/
#add -Imingw-std-threads to CXXFLAGS

src_dir := src

ifndef os
os := $(shell uname -s)
endif
ifndef arch
arch := $(shell uname -m)
endif

ifndef graphics
graphics := QT
endif

CC := $(crossp)cc
CXX := $(crossp)c++
GPERF ?= gperf
ifeq ($(os),Linux)
lib_suffix=.so
boost_thread = boost_thread
DYNLIB=-shared
CFLAGS ?= -fpic -g -MMD -Wall
endif
ifeq ($(os),Darwin)
lib_suffix=.dylib
boost_thread = boost_thread-mt
DYNLIB=-dynamiclib
CFLAGS ?= -g -MMD -Wall
endif
CXXFLAGS ?= $(CFLAGS) -std=c++14
ifeq ($(os),win64)
CXXFLAGS += -I$(src_dir)/core/ext/mingw-std-threads
endif
LDFLAGS ?= -l$(boost_thread) -lboost_chrono -lboost_system -L$(build_dir)
tidy := /usr/local/Cellar/llvm/5.0.1/bin/clang-tidy
tidy_opts := -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk 

srcs :=
objs :=
libs :=
deps :=
cov :=

define lib_makerule

lib_srcs :=
lib_srcgens :=
lib_objs :=
lib_cppflags :=
lib_cflags :=
lib_ldflags :=
lib_djnn_deps := 

# possibly override default
-include $$(src_dir)/$1/djnn-lib.mk

# default
$1_gperf_srcs :=
#$$(shell find src/$1 -name "*.gperf")
$1_cpp_srcs := $$(filter %.cpp,$$(lib_srcs))
$1_c_srcs := $$(filter %.c,$$(lib_srcs))
$1_objs := $$($1_cpp_srcs:.cpp=.o) $$($1_c_srcs:.c=.o)
$1_objs := $$(addprefix $(build_dir)/, $$($1_objs))
$1_cov_gcno  := $$($1_objs:.o=.gcno)
$1_cov_gcda  := $$($1_objs:.o=.gcda)

$1_srcgens := $$(lib_srcgens)
$1_objs += $$(lib_objs)

$1_deps := $$($1_objs:.o=.d)
$1_lib := $$(build_dir)/libdjnn-$1$$(lib_suffix)
$1_lib_cppflags := $$(lib_cppflags)
$1_lib_cflags := $$(lib_cflags)
$1_lib_ldflags := $$(lib_ldflags)
$1_lib_all_ldflags := $$($1_lib_ldflags)

ifeq ($(os),Darwin)
ifdef lib_djnn_deps
$1_djnn_deps := $$(addsuffix $$(lib_suffix),$$(addprefix $$(build_dir)/libdjnn-,$$(lib_djnn_deps)))
$1_lib_all_ldflags += $$(foreach lib,$$(lib_djnn_deps), $$(value $$(lib)_lib_ldflags))
endif
endif

$$($1_objs): CXXFLAGS+=$$($1_lib_cppflags)
$$($1_objs): CFLAGS+=$$($1_lib_cflags)
$$($1_lib): LDFLAGS+=$$($1_lib_all_ldflags)
$$($1_lib): $$($1_djnn_deps)

$$($1_lib): $$($1_objs)
	@mkdir -p $$(dir $$@)
	$$(CXX) $(DYNLIB) -o $$@ $$^ $$(LDFLAGS)


$1_tidy_srcs := $$(addsuffix _tidy,$$($1_srcs))
$$($1_tidy_srcs): tidy_opts+=$$($1_lib_cppflags)

$1_tidy: $$($1_tidy_srcs)
.PHONY: $1_tidy

$1_dbg:
	@echo $1_dbg
	@echo $$($1_cpp_srcs)
	@echo $$($1_c_srcs)
	@echo $$($1_objs)
	@echo $$($1_djnn_deps)
	@echo $$($1_lib_ldflags)
	@echo $$($1_lib_all_ldflags)
	@echo $$($1_cov_gcno)
	@echo $$($1_cov_gcda)

srcs += $$($1_srcs)
srcgens += $$($1_srcgens)
objs += $$($1_objs)
deps += $$($1_deps)
libs += $$($1_lib)
cov  += $$($1_cov_gcno) $$($1_cov_gcda) $(lcov_file)

endef

djnn_libs := core base comms display gui animation input
$(foreach a,$(djnn_libs),$(eval $(call lib_makerule,$a)))

#headers := $(foreach a,$(djnn_libs),$a/$a)
headers := $(djnn_libs)
headers := $(addsuffix .h,$(headers)) $(addsuffix -dev.h,$(headers))
headers := $(addprefix $(build_dir)/include/djnn/,$(headers))

headers: $(headers)
.PHONY: headers

djnn: $(libs) $(headers)
.PHONY: djnn

$(build_dir)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(build_dir)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# for generated .cpp
$(build_dir)/%.o: $(build_dir)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(build_dir)/include/djnn/%.h: src/*/%.h
	@mkdir -p $(dir $@)
	cp $< $@

%_tidy: %
	$(tidy) -header-filter="djnn" -checks="*" -extra-arg=-std=c++14 $^ -- $(tidy_opts)
.PHONY: %_tidy

all_tidy := $(addsuffix _tidy,$(srcs))
tidy: $(all_tidy)
.PHONY: tidy

-include $(deps)

pre_cov: CXXFLAGS += --coverage
pre_cov: LDFLAGS += --coverage
pre_cov : djnn
.PHONY: pre_cov

cov:
	lcov -o $(lcov_file) -c -d . -b . --no-external > /dev/null 2>&1
	lcov --remove $(lcov_file) '*/ext/*' -o $(lcov_file)
	genhtml -o $(lcov_output_dir) $(lcov_file)
	cd $(lcov_output_dir) ; open index.html
.PHONY: cov

clean:
	rm -f $(deps) $(objs) $(libs) $(srcgens) $(cov)
	rm -rf $(lcov_output_dir) > /dev/null 2>&1 || true
	rmdir $(build_dir) > /dev/null 2>&1 || true
.PHONY: clean

distclean clear:
	rm -rf $(build_dir)
.PHONY: distclean clear

dbg:
	@echo $(libs)
.PHONY: dbg

ifeq ($(os),Darwin)
#https://brew.sh/
pkgdeps := expat curl
pkgdeps += expat qt5
#pkgdeps += expat freetype sdl2
pkgcmd := brew install
endif

ifeq ($(os),win64)
#https://www.msys2.org/
pkgdeps := expat curl
pkgdeps += qt5 
#-static
#pkgdeps += freetype SDL2
pkdepgs := $(addprefix mingw-w64-i686-,$(pkgdeps))
pkgcmd := pacman -S
endif

install-pkgdeps:
	$(pkgcmd) $(pkgdeps)
.PHONY: install-deps


