#	djnn cpp
#
#	The copyright holders for the contents of this file are:
#		Ecole Nationale de l'Aviation Civile, France (2017-2020)
#	See file "license.terms" for the rights and conditions
#	defined by copyright holders.
#
#
#	Contributors:
#		Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
#		Stéphane Conversy <stephane.conversy@enac.fr>
#		Mathieu Poirier	  <mathieu.poirier@enac.fr>

# remove builtin rules: speed up build process and help debug
MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

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

src_dir ?= src
build_dir ?= build
display ?= QT
#options: QT SDL
graphics ?= QT
#options: QT CAIRO GL

# cross-compile support
ifndef cross_prefix
cross_prefix := c
#options: c g llvm-g i686-w64-mingw32- arm-none-eabi- em
#/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-c
#/usr/local/Cellar/android-ndk/r14/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-g
endif

CXX := $(cross_prefix)++

ifeq ($(cross_prefix),c)
CC := $(cross_prefix)c
else ifeq ($(cross_prefix),g)
CC := $(cross_prefix)cc
else ifeq ($(cross_prefix),em)
CC := $(cross_prefix)cc
else
CC := $(cross_prefix)	
endif

ifeq ($(cross_prefix),g)
CXXFLAGS += -Wno-psabi #https://stackoverflow.com/a/48149400
endif

ifndef os
os := $(shell uname -s)
endif
ifndef arch
arch := $(shell uname -m)
endif

ifeq ($(findstring MINGW,$(os)),MINGW)
os := MinGW
endif


GPERF ?= gperf


CFLAGS += -I$(src_dir)

#CFLAGS += -fsanitize=thread -O1
#LDFLAGS += -fsanitize=thread

#CFLAGS += -fsanitize=address -O1
#LDFLAGS += -fsanitize=address

#CFLAGS += -fsanitize=memory -O1
#LDFLAGS += -fsanitize=memory

# scan_build: static analizer
# 1. download the latest llvm (+9.0.0) release from your platform using "Pre-Built Binaries" on http://releases.llvm.org/download.html
# 2. launch the CLI on this Makefile wih: 
#	/path/to/llvm/bin/scan-build -o build/scan-report/htmldir make -j
# 3. vizualize report with CLI (this command is given at the end of the above command: 
#	/path/to/llvm/bin/scan-view build/scan-report/htmldir/xxxx-xx-xx-xxxxx-xxxxx-x 

lib_static_suffix = .a

ifeq ($(os),Linux)
lib_suffix =.so
boost_libs = -lboost_thread -lboost_chrono -lboost_system
#-lboost_fiber-mt -lboost_context-mt
DYNLIB = -shared
CFLAGS += -fpic -g -MMD -Wall
LDFLAGS += -L$(build_dir)
YACC = bison -d
endif

ifeq ($(os),Darwin)
lib_suffix =.dylib
boost_libs = -lboost_thread-mt -lboost_chrono-mt -lboost_system-mt -lboost_fiber-mt -lboost_context-mt
DYNLIB = -dynamiclib
CFLAGS += -g -MMD -Wall
LDFLAGS += -L$(build_dir)
YACC = /usr/local/opt/bison/bin/bison -d
LEX = /usr/local/opt/flex/bin/flex
CXXFLAGS += -I/usr/local/opt/flex/include
LDFLAGS += -L/usr/local/opt/flex/lib
endif

# for windows with mingwXX
ifeq ($(os),MinGW)
lib_suffix =.dll
boost_libs = -lboost_thread-mt -lboost_chrono-mt -lboost_system-mt
DYNLIB = -shared
CFLAGS += -fpic -g -MMD -Wall
CXXFLAGS += -I/usr/include # Fix for FlexLexer.h in /usr/include and in /ming64/include
LDFLAGS += -L$(build_dir)
YACC = bison -d
endif

ifeq ($(os),FreeRTOS)
lib_suffix =.so
boost_libs =
DYNLIB = -shared
CFLAGS += -fpic -g -MMD -Wall
LDFLAGS += -L$(build_dir)
endif

ifeq ($(findstring android,$(cross_prefix)),android)
os := android
inc_android =  \
-DSDL_DISABLE_IMMINTRIN_H \
-I/usr/local/Cellar/android-ndk/r14/sources/cxx-stl/llvm-libc++/include \
-I/usr/local/Cellar/android-ndk/r14/platforms/android-24/arch-arm/usr/include \
-I../ext-libs/android/libexpat/expat/lib \
-I../ext-libs/android/curl/include \
-I../ext-libs/boost_1_68_0 \
-I../ext-libs/fontconfig \
-I/usr/local/include \
--sysroot=/usr/local/Cellar/android-ndk/r14/platforms/android-24/arch-arm
#/usr/local/include for glm

CFLAGS += $(inc_android)

LDFLAGS = -L../ext-libs/android/libexpat/expat/lib/.libs \
-L../ext-libs/android/curl/lib/.libs \
--sysroot=/usr/local/Cellar/android-ndk/r14/platforms/android-24/arch-arm
endif


ifeq ($(display),QT)

ifeq ($(os),Darwin)
thread ?= BOOST
chrono ?= STD
else
thread ?= QT
chrono ?= STD
endif

else ifeq ($(display),SDL)
thread ?= SDL
chrono ?= STD

else ifeq ($(display),DRM)
thread ?= STD
chrono ?= STD

else ifeq ($(display),)
thread ?= STD
chrono ?= STD

else
$(warning "unknown display (choose among: QT, SDL, DRM, (none))")

endif

thread ?= BOOST
#options: QT SDL STD FIBER
chrono ?= STD
#options: QT FIBER STD

ifeq ($(thread),BOOST)
CXXFLAGS += -DDJNN_USE_BOOST_THREAD=1
LDFLAGS += $(boost_libs)
ifeq ($(os),Darwin)
CXXFLAGS += -I/usr/local/include
endif

else ifeq ($(thread),QT)
CXXFLAGS += -DDJNN_USE_QT_THREAD=1

else ifeq ($(thread),SDL)
CXXFLAGS += -DDJNN_USE_SDL_THREAD=1
endif


ifeq ($(chrono),BOOST)
CXXFLAGS += -DDJNN_USE_BOOST_CHRONO=1
ifneq ($(thread),BOOST)
ifeq ($(os),Darwin)
CXXFLAGS += -I/usr/local/include
endif
LDFLAGS += $(boost_libs)
endif

else ifeq ($(chrono),STD)
CXXFLAGS += -DDJNN_USE_STD_CHRONO=1

else ifeq ($(chrono),SDL)
CXXFLAGS += -DDJNN_USE_SDL_CHRONO=1
endif

#is chrono useless now?

CXXFLAGS += -std=c++14
CXXFLAGS += $(CFLAGS)

$(build_dir)/src/gui/css-parser/scanner.o: CXXFLAGS += -Dregister=""

ifeq ($(cross_prefix),em)
os := em
DYNLIB=
lib_suffix=.bc

EMFLAGS := -Wall -Wno-unused-variable -Oz \
-s USE_BOOST_HEADERS -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_FREETYPE=1 -s USE_WEBGL2=1 \
-DSDL_DISABLE_IMMINTRIN_H \
-s EXPORT_ALL=1 -s DISABLE_EXCEPTION_CATCHING=0 \
-s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1 \
-s ASSERTIONS=2 \
-s ERROR_ON_UNDEFINED_SYMBOLS=0

em_ext_libs_path ?= ../djnn-emscripten-ext-libs

##idn2 expat curl fontconfig unistring psl
# to add in application makefile:
#ext_libs := expat curl
#ext_libs := $(addprefix $(em_ext_libs_path)/lib/lib,$(addsuffix .a, $(ext_libs))) -lopenal

EMCFLAGS += $(EMFLAGS) -I$(em_ext_libs_path)/include -I/usr/local/include #glm
CFLAGS += $(EMCFLAGS)
CXXFLAGS += $(EMCFLAGS)
LDFLAGS += $(EMFLAGS) \
	--emrun
#$(ext_libs) # to add in application makefile

picking ?= ANALYTICAL
endif

ifeq ($(picking),ANALYTICAL)
CXXFLAGS += -DDJNN_USE_ANALYTICAL_PICKING
else ifeq ($(picking),COLOR)
CXXFLAGS += -DDJNN_USE_COLOR_PICKING
else ifeq ($(picking),)
CXXFLAGS += -DDJNN_USE_COLOR_PICKING
else
$(warning("unkown picking method"))
endif

tidy := /usr/local/Cellar/llvm/5.0.1/bin/clang-tidy
tidy_opts := -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk 

lcov_file ?= $(build_dir)/djnn_cov.info
lcov_output_dir ?= $(build_dir)/coverage_html


djnn_libs ?= core exec_env base comms display gui input animation utils files audio extra

ifeq ($(cross_prefix),em)
djnn_libs := core exec_env base display gui animation utils audio input
# comms input
endif

#ifeq ($(os),MinGW)
#djnn_libs := core base comms display gui input animation utils files audio
#endif

ifeq ($(physics), ODE)
djnn_libs += physics
endif

ifeq ($(findstring avr,$(cross_prefix)),avr)
djnn_libs := core base
CXXFLAGS += -I/Applications/Arduino.app/Contents/Java/hardware/tools/avr/avr -I/usr/local/include
#https://github.com/andysworkshop/avr-stl/releases
endif

srcs :=
objs :=
libs :=
libs_static :=
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
lib_pkg :=

# possibly override default
-include $$(src_dir)/$1/djnn-lib.mk

# default
$1_gperf_srcs :=
$1_cpp_srcs := $$(filter %.cpp,$$(lib_srcs))
$1_c_srcs := $$(filter %.c,$$(lib_srcs))
$1_objs := $$($1_cpp_srcs:.cpp=.o) $$($1_c_srcs:.c=.o)
$1_objs := $$(addprefix $(build_dir)/, $$($1_objs))
$1_cov_gcno  := $$($1_objs:.o=.gcno)
$1_cov_gcda  := $$($1_objs:.o=.gcda)
$1_lib_pkg := $$(lib_pkg)

$1_srcgens := $$(lib_srcgens)
$1_objs += $$(lib_objs)

$1_pkg_deps :=
$1_deps := $$($1_objs:.o=.d)
$1_libname := libdjnn-$1$$(lib_suffix)
$1_lib := $$(build_dir)/$$($1_libname)
$1_libname_static := libdjnn-$1$$(lib_static_suffix)
$1_lib_static := $$(build_dir)/$$($1_libname_static)
$1_lib_cppflags := $$(lib_cppflags)
$1_lib_cflags := $$(lib_cflags)
$1_lib_ldflags := $$(lib_ldflags)

ifneq ($$($1_lib_pkg),)
$1_lib_cflags += $$(shell pkg-config --cflags $$($1_lib_pkg))
$1_lib_ldflags += $$(shell pkg-config --libs $$($1_lib_pkg))
endif

$1_lib_all_ldflags := $$($1_lib_ldflags)

ifeq ($(os),$(filter $(os),Darwin MinGW))
ifdef lib_djnn_deps
$1_djnn_deps := $$(addsuffix $$(lib_suffix),$$(addprefix $$(build_dir)/libdjnn-,$$(lib_djnn_deps)))
$1_lib_all_ldflags += $$(addprefix -ldjnn-,$$(lib_djnn_deps)) $$(foreach lib,$$(lib_djnn_deps), $$(value $$(lib)_lib_ldflags))
endif
endif

ifneq ($(os), em)
$1_lib_rpath := -Wl,-rpath,$$($1_libname)
endif
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

$$($1_lib_static): $$($1_objs)
	@mkdir -p $$(dir $$@)
	#ar rU $$@ $$($1_objs)
	/usr/bin/ar -r -u $$@ $$($1_objs)

$1_tidy_srcs := $$(addsuffix _tidy,$$($1_srcs))
$$($1_tidy_srcs): tidy_opts+=$$($1_lib_cppflags)

$1_tidy: $$($1_tidy_srcs)
.PHONY: $1_tidy

$1_dbg:
	@echo $$($1_objs)
	@echo $$($1_djnn_deps)
	@echo $$($1_lib_ldflags)

$1_tructruc:
	@echo $$($1_lib_pkg)
	@echo $$($1_lib_ldflags)


$1_atchoum:
	@echo $1_dbg
	@echo $$($1_cpp_srcs)
	@echo $$($1_c_srcs)
	@echo $$($1_objs)
	@echo $$($1_lib_all_ldflags)
	@echo $$($1_cov_gcno)
	@echo $$($1_cov_gcda)

srcs += $$($1_srcs)
srcgens += $$($1_srcgens)
objs += $$($1_objs)
deps += $$($1_deps)
libs += $$($1_lib)
libs_static += $$($1_lib_static)
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

static: $(libs_static) $(headers)
.PHONY: static

size:
	size -t $(libs_static)
.PHONY: size

strip:
	strip $(libs_static)

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

$(build_dir)/%.cpp $(build_dir)/%.hpp: %.y
	@mkdir -p $(dir $@)
	$(YACC) -v -o $@ $<

$(build_dir)/%.cpp: %.l
	@mkdir -p $(dir $@)
	$(LEX) -o $@ $<

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
	lcov -d $(build_dir) -b . --zerocounters
.PHONY: pre_cov

cov_jenkins:
	lcov -o $(lcov_file) -c -d . -b . --no-external > /dev/null 2>&1
	# remove .cpp from css-parser which are destroyed after compilation
	lcov --remove $(lcov_file) '*/css-parser/*.cpp' -o $(lcov_file)
	# convert lcov report to gcov report
	# gcovr --xml-pretty -o $(build_dir)/gcov_report.xml -s
	genhtml -o $(lcov_output_dir) $(lcov_file)
cov: cov_jenkins
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
	@echo $(src_dir)

.PHONY: dbg

pkgdeps += bison flex

ifeq ($(os),Linux)
pkgcmd := apt install -y
pkgupg := apt upgrade -y

pkgdeps += libexpat1-dev libcurl4-openssl-dev libudev-dev gperf libboost-thread-dev libevdev-dev libopenal-dev #libboost-fiber-dev
ifeq ($(display),QT)
pkgdeps += qt5-default
endif
ifeq ($(display),SDL)
pkgdeps += libfontconfig1-dev
ifeq ($(specialtarget),raspberry-ua-netinst)
# on rpi, compile and install sdl2 with KMSDRM support, raspbian's sdl2 does not provide it by default
#pkgdeps += libraspberrypi-dev
else
pkgdeps += libsdl2-dev libsdl2-image-dev
endif
endif
ifeq ($(display),DRM)
pkgdeps += libdrm-dev
endif
ifeq ($(graphics),CAIRO)
pkgdeps += libcairo-dev libpango1.0-dev #libpangocairo-1.0-0 
endif
ifeq ($(graphics),GL)
pkgdeps += libfreetype6-dev libglm-dev
endif
endif

ifeq ($(os),Darwin)
#https://brew.sh/
pkgcmd := brew install
pkgupg := brew upgrade

pkgdeps += expat curl boost
pkgdeps += qt5
pkgdeps += cairo pango
pkgdeps += sdl2 sdl2_image
ifeq ($(graphics),GL)
pkgdeps += glm
endif
endif

ifeq ($(os), MinGW)
#https://www.msys2.org/
#pkgdeps := git make pkg-config
pkgcmd := pacman -S --needed
pkgupg := pacman -Syu

mgwpkgdeps += gcc boost expat curl qt5
mgwpkgdeps += freetype SDL2 SDL2_image cairo pango fontconfig
ifeq ($(graphics),GL)
mgwpkgdeps += glm
endif
mgwpkgdeps := $(addprefix mingw-w64-x86_64-, $(mgwpkgdeps))
pkgdeps += $(mgwpkgdeps)
endif

install-pkgdeps:
	$(pkgcmd) $(pkgdeps)

upgrade-pkgdeps:
	$(pkgupg) $(pkgdeps)

.PHONY: install-pkgdeps update-pkgdeps

# mingw on mac:
#brew install mingw-w64, git clone https://github.com/meganz/mingw-std-threads, https://bitbucket.org/skunkos/qt5-minimalistic-builds/downloads/
#add -Imingw-std-threads to CXXFLAGS

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

