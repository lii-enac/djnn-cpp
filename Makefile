#	djnn cpp
#
#	The copyright holders for the contents of this file are:
#		Ecole Nationale de l'Aviation Civile, France (2017-2018)
#	See file "license.terms" for the rights and conditions
#	defined by copyright holders.
#
#
#	Contributors:
#		Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
#		Stéphane Conversy <stephane.conversy@enac.fr>
#		Mathieu Poirier	  <mathieu.poirier@enac.fr>


default: all
.PHONY: default

all: config.mk djnn
.PHONY: all

help:
	@echo "default: djnn ; all: djnn"
	@echo "experiment make -j !!"



config.mk:
	cp config.default.mk config.mk

include config.default.mk
-include config.mk

# remove builtin rules: speed up build process and help debug
MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

ifndef os
os := $(shell uname -s)
endif
ifndef arch
arch := $(shell uname -m)
endif

# cross-compile support
ifndef cross_prefix
cross_prefix := llvm-g

ifeq ($(os),Linux)
cross_prefix := g
endif
#cross_prefix := arm-none-eabi-
#cross_prefix := /Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-c
#cross_prefix := em
#cross_prefix := i686-w64-mingw32-
#brew install mingw-w64, git clone https://github.com/meganz/mingw-std-threads, https://bitbucket.org/skunkos/qt5-minimalistic-builds/downloads/
#add -Imingw-std-threads to CXXFLAGS

#cross_prefix := /usr/local/Cellar/android-ndk/r14/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-g
endif

osmingw := MINGW64_NT-10.0
#osmingw := MINGW64_NT-6.1
#osmingw := MINGW32_NT-6.1

src_dir := src

graphics ?= QT
CC := $(cross_prefix)cc
CXX := $(cross_prefix)++
GPERF ?= gperf

thread ?= BOOST
# FIBER STD
chrono ?= BOOST
# FIBER STD


ifeq ($(os),Linux)
lib_suffix=.so
boost_libs = -lboost_thread -lboost_chrono -lboost_system
DYNLIB=-shared
CFLAGS ?= -fpic -g -MMD -O0 -Wall
LDFLAGS ?= $(boost_libs) -L$(build_dir)
endif

ifeq ($(os),Darwin)
lib_suffix=.dylib
boost_libs = -lboost_thread-mt -lboost_chrono-mt -lboost_system-mt -lboost_fiber-mt -lboost_context-mt
DYNLIB=-dynamiclib
CFLAGS ?= -g -MMD #-Wall
LDFLAGS ?= $(boost_libs) -L$(build_dir)
endif

# for windows with mingw64 
ifeq ($(os),$(osmingw))
lib_suffix=.dll
boost_libs = -lboost_thread-mt -lboost_chrono-mt -lboost_system-mt
dynlib =-shared
CFLAGS ?= -fpic -g -MMD -Wall
LDFLAGS ?= $(boost_libs) -L$(build_dir)
endif
	
ifeq ($(findstring android,$(cross_prefix)),android)
CXXFLAGS := \
-I/usr/local/Cellar/android-ndk/r14//sources/cxx-stl/llvm-libc++/include \
-I/usr/local/include \
-DSDL_DISABLE_IMMINTRIN_H \
$(CXXFLAGS)

CFLAGS := $(CFLAGS) -I/usr/local/Cellar/android-ndk/r14/platforms/android-24/arch-arm/usr/include

endif

ifeq ($(cross_prefix),em)
EMFLAGS := -Wall -Oz -s WASM=0 -s USE_SDL=2 -s FULL_ES2=1 -s USE_FREETYPE=1 \
-s EXPORT_ALL=1 -s ASSERTIONS=1 -s DISABLE_EXCEPTION_CATCHING=0 \
-s TOTAL_MEMORY=1GB

#-s USE_PTHREADS=1 -s PROXY_TO_PTHREAD=1 \

#TOTAL_MEMORY?
#-s ALLOW_MEMORY_GROWTH=1 
#-s WASM=1 
#-s ALLOW_MEMORY_GROWTH=1 

lib_suffix=.bc
#boost_libs = -lboost_thread -lboost_chrono -lboost_system
#boost_libs = \
#	../boost_1_68_0/bin.v2/libs/chrono/build/emscripten-1.38.12/debug/cxxstd-14-iso/link-static/libboost_chrono.bc \
#	../boost_1_68_0/bin.v2/libs/system/build/emscripten-1.38.12/debug/cxxstd-14-iso/link-static/libboost_system.bc \
#	../boost_1_68_0/bin.v2/libs/thread/build/emscripten-1.38.12/debug/cxxstd-14-iso/link-static/threadapi-pthread/threading-multi/libboost_thread.bc

CFLAGS += $(EMFLAGS) \
	-I../ext-libs/libexpat/expat/lib \
	-I../ext-libs/curl/include \
	-I../ext-libs/boost_1_68_0 \
	-I/usr/local/include #glm
os := em

LDFLAGS = $(EMFLAGS)

	# ../ext-libs/libexpat/expat/lib/.libs/libexpat.dylib \
	# ../ext-libs/curl/lib/.libs/libcurl.dylib \
	# ../ext-libs/fontconfig/src/.libs/libfontconfig.dylib \
	# ../ext-libs/boost_1_68_0/bin.v2/libs/chrono/build/emscripten-1.38.21/debug/cxxstd-14-iso/link-static/libboost_chrono.bc \
	# ../ext-libs/boost_1_68_0/bin.v2/libs/system/build/emscripten-1.38.21/debug/cxxstd-14-iso/link-static/libboost_system.bc \
#see smala Makefile
#	../ext-libs/boost_1_68_0/bin.v2/libs/thread/build/emscripten-1.38.21/debug/cxxstd-14-iso/link-static/threadapi-pthread/threading-multi/libboost_thread.bc \

#https://stackoverflow.com/questions/15724357/using-boost-with-emscripten
# ./bootstrap.sh
# emacs project-config.jam
# using darwin : : em++ ;
# ./b2 cxxstd=14 toolset=emscripten link=static thread chrono system

endif

	
ifeq ($(findstring android,$(cross_prefix)),android)
CXXFLAGS := \
-I/usr/local/Cellar/android-ndk/r14//sources/cxx-stl/llvm-libc++/include \
-I/usr/local/include \
-DSDL_DISABLE_IMMINTRIN_H \
$(CXXFLAGS)

CFLAGS := $(CFLAGS) -I/usr/local/Cellar/android-ndk/r14/platforms/android-24/arch-arm/usr/include

endif

CXXFLAGS := $(CXXFLAGS) $(CFLAGS) -std=c++14 \
-DDJNN_USE_BOOST_THREAD=1 -DDJNN_USE_BOOST_CHRONO=1

tidy := /usr/local/Cellar/llvm/5.0.1/bin/clang-tidy
tidy_opts := -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk 

lcov_file ?= $(build_dir)/djnn_cov.info
lcov_output_dir ?= $(build_dir)/coverage_html


ifeq ($(os),$(osmingw))
djnn_libs := core base display gui animation # comms input
else
djnn_libs := core base comms display gui input animation
endif

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
$1_libname := libdjnn-$1$$(lib_suffix)
$1_lib := $$(build_dir)/$$($1_libname)
$1_lib_cppflags := $$(lib_cppflags)
$1_lib_cflags := $$(lib_cflags)
$1_lib_ldflags := $$(lib_ldflags)
$1_lib_all_ldflags := $$($1_lib_ldflags)

ifeq ($(os),$(filter $(os),Darwin MINGW64_NT-10.0))
ifdef lib_djnn_deps
$1_djnn_deps := $$(addsuffix $$(lib_suffix),$$(addprefix $$(build_dir)/libdjnn-,$$(lib_djnn_deps)))
$1_lib_all_ldflags += $$(addprefix -ldjnn-,$$(lib_djnn_deps)) $$(foreach lib,$$(lib_djnn_deps), $$(value $$(lib)_lib_ldflags))
endif
endif

$1_lib_rpath := -Wl,-rpath,$$($1_libname)
ifeq ($(os), Darwin)
$1_lib_rpath := -Wl,-install_name,$$($1_libname),
endif

$$($1_objs): CXXFLAGS+=$$($1_lib_cppflags)
$$($1_objs): CFLAGS+=$$($1_lib_cflags)
$$($1_lib): LDFLAGS+=$$($1_lib_all_ldflags)
$$($1_lib): $$($1_djnn_deps)

$$($1_lib): $$($1_objs)
	@mkdir -p $$(dir $$@)
	$$(CXX) $(DYNLIB) -o $$@ $$($1_objs) $$(LDFLAGS) $$($1_lib_rpath)

$1_tidy_srcs := $$(addsuffix _tidy,$$($1_srcs))
$$($1_tidy_srcs): tidy_opts+=$$($1_lib_cppflags)

$1_tidy: $$($1_tidy_srcs)
.PHONY: $1_tidy

$1_dbg:
	@echo $$($1_objs)
	@echo $$($1_djnn_deps)
	@echo $$($1_lib_ldflags)

$1_tructruc:
	#@echo $1_dbg
	#@echo $$($1_cpp_srcs)
	#@echo $$($1_c_srcs)
	#@echo $$($1_objs)
	@echo $$($1_lib_all_ldflags)
	#@echo $$($1_cov_gcno)
	#@echo $$($1_cov_gcda)

srcs += $$($1_srcs)
srcgens += $$($1_srcgens)
objs += $$($1_objs)
deps += $$($1_deps)
libs += $$($1_lib)
cov  += $$($1_cov_gcno) $$($1_cov_gcda) $(lcov_file)

endef

$(foreach a,$(djnn_libs),$(eval $(call lib_makerule,$a)))

#headers := $(foreach a,$(djnn_libs),$a/$a)
headers := $(djnn_libs)
headers := $(addsuffix .h,$(headers)) $(addsuffix -dev.h,$(headers))
headers := $(addprefix $(build_dir)/include/djnn/,$(headers))

headers: $(headers)
.PHONY: headers

djnn: $(libs) $(headers)
.PHONY: djnn

# config_header_files := $(build_dir)/src/core/syshook/cpp-thread-config.h $(build_dir)/src/core/syshook/cpp-chrono-config.h
# config_headers: $(config_header_files)
# .PHONY: config_headers
# #.PHONY: $(config_header_files)

# $(build_dir)/src/core/syshook/cpp-thread-config.h: $(FORCE)
# 	@$(eval config.h = $(shell mktemp /tmp/config.h.XXXXX))
# 	@echo "#define DJNN_USE_"$(thread)"_THREAD 1" > $(config.h)
# 	@if ! diff -q $(config.h) $@ &>/dev/null; then echo "cp $(config.h) $@"; cp $(config.h) $@; fi
# 	@rm $(config.h)
# $(build_dir)/src/core/syshook/cpp-chrono-config.h: $(FORCE)
# 	@$(eval config.h = $(shell mktemp /tmp/config.h.XXXXX))
# 	@echo "#define DJNN_USE_"$(chrono)"_CHRONO 1" > $(config.h)
# 	@if ! diff -q $(config.h) $@ &>/dev/null; then echo "cp $(config.h) $@"; cp $(config.h) $@;  fi
# 	@rm $(config.h)


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
	@echo $(os)
.PHONY: dbg

ifeq ($(os),Linux)
#https://brew.sh/
pkgdeps := libexpat1-dev libcurl4-openssl-dev libudev-dev gperf libboost-thread-dev libevdev-dev
pkgdeps += qt5-default
#pkgdeps += freetype sdl2
pkgcmd := apt install -y
endif

ifeq ($(os),Darwin)
#https://brew.sh/
pkgdeps := expat curl boost
pkgdeps += expat qt5
#pkgdeps += freetype sdl2
pkgcmd := brew install
endif

ifeq ($(os),MINGW64_NT-10.0)
#https://www.msys2.org/
#pkgdeps := git make
pkgdeps := pkg-config gcc boost expat curl qt5
#pkgdeps += freetype SDL2
pkgdeps := $(addprefix mingw-w64-x86_64-, $(pkgdeps))
pkgcmd := pacman -S
endif

install-pkgdeps:
	$(pkgcmd) $(pkgdeps)
.PHONY: install-pkgdeps


