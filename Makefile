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

# ---------------------------------------
# default

default: all
.PHONY: default

all: config.mk dirs djnn pkgconf


help:
	@echo "default: djnn ; all: djnn"
	@echo "experiment make -j !!"

config.mk:
	cp config.default.mk config.mk

MAJOR = 1
MINOR = 12
MINOR2 = 0

include config.default.mk
-include config.mk

src_dir ?= src
build_dir ?= build
display ?= QT #options: QT SDL
graphics ?= QT #options: QT CAIRO GL

RANLIB ?= ranlib
#GPERF ?= gperf
SIZE ?= size

# ---------------------------------------
# utils

# https://stackoverflow.com/a/16151140
uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))

# recursive wildcard https://stackoverflow.com/a/12959694
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
rwildcardmul = $(wildcard $(addsuffix $2, $1)) $(foreach d,$(wildcard $(addsuffix *, $1)),$(call rwildcard,$d/,$2))

all_srcs = $(call rwildcard,src/,*)
all_dirs = $(call uniq,$(dir $(all_srcs)))

# ---------------------------------------
# directory structure

$(build_dir)/%/: %/
	@mkdir -p $@

$(build_dir)/lib:
	@mkdir -p $@

$(build_dir)/include/djnn-cpp:
	@mkdir -p $@

dirs: $(build_dir)/lib $(build_dir)/include/djnn-cpp $(addprefix $(build_dir)/,$(all_dirs)) 


# ---------------------------------------
# stylized actions

ifeq ($V,0)
rule_message =
else
rule_message = $(echo) $1 $2
endif

ifeq ($V,)
stylized_target = "\\033[1m$(notdir $@)\\033[0m"
else ifeq ($V,0)
#stylized_target = "\\033[1m$(notdir $@)\\033[0m"
else ifeq ($V,1)
stylized_target = "\\033[1m$(notdir $@)\\033[0m"
else ifeq ($V,2)
stylized_target = "\\033[1m$(notdir $@)\\033[0m" \(older than $(notdir $?)\)
else ifeq ($V,3)
stylized_target = "\\033[1m$@\\033[0m" \(older than $?\)
endif
#in case it doesn't work on some terminals:
#stylized_target = $(notdir $@) \(older than $(notdir $?)\)


# ---------------------------------------
# cross-compile support

ifdef cross_prefix
#cross_prefix := c
#options: c g llvm-g i686-w64-mingw32- arm-none-eabi- em
#/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-c
#/usr/local/Cellar/android-ndk/r14/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-g
ifneq ($(cross_prefix),g)
CC := $(cross_prefix)$(CC)
CXX := $(cross_prefix)$(CXX)
AR := $(cross_prefix)$(AR)
RANLIB := $(cross_prefix)$(RANLIB)
SIZE := $(cross_prefix)$(SIZE)
else
#temporary covid hack
CC := gcc
CXX := g++
#AR := $(cross_prefix)$(AR)
RANLIB := ranlib
SIZE ?=
endif
endif

# ---------------------------------------
# os and compiler specifics

ifndef os
os := $(shell uname -s)
endif
ifndef arch
arch := $(shell uname -m)
endif

ifeq ($(findstring MINGW,$(os)),MINGW)
os := MinGW
endif

build_lib_dir := $(build_dir)/lib
CFLAGS += -I$(src_dir)
build_incl_dir := $(build_dir)/include/djnn-cpp
LDFLAGS += -L$(build_lib_dir)
lib_static_suffix = .a
echo = echo -e

ifeq ($(os),Linux)
CFLAGS += -fpic -g -MMD -Wall
#CXXFLAGS += -Wno-psabi #https://stackoverflow.com/a/48149400
lib_suffix =.so
DYNLIB = -shared
YACC = bison -d
endif

ifeq ($(os),Darwin)
CFLAGS += -g -MMD -Wall -Wno-deprecated-declarations
lib_suffix =.dylib
DYNLIB = -dynamiclib
echo = echo
#AR ?= /usr/bin/ar
#ARFLAGS ?= -r
endif

ifeq ($(os),MinGW)
CFLAGS += -fpic -g -MMD -Wall
CXXFLAGS += -D_USE_MATH_DEFINES # https://docs.microsoft.com/en-us/cpp/c-runtime-library/math-constants?view=vs-2019
CXXFLAGS += -I/usr/include # Fix for FlexLexer.h in /usr/include and in /ming64/include
#CXXFLAGS += -Wno-psabi #https://stackoverflow.com/a/48149400
lib_suffix =.dll
DYNLIB = -shared
YACC = bison -d
endif

ifeq ($(os),FreeRTOS)
CFLAGS += -fpic -g -MMD -Wall -Os -DDJNN_NO_DEBUG -DDJNN_NO_SERIALIZE
CFLAGS += -DDJNN_USE_FREERTOS=1
#CXXFLAGS += -Wno-psabi #https://stackoverflow.com/a/48149400
lib_suffix =.so
DYNLIB = -shared
endif

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

EMCFLAGS += $(EMFLAGS) -I$(em_ext_libs_path)/include -I/usr/local/include #glm
CFLAGS += $(EMCFLAGS)
CXXFLAGS += $(EMCFLAGS)
LDFLAGS += $(EMFLAGS) \
	--emrun
#   $(ext_libs) # to add in application makefile
##idn2 expat curl fontconfig unistring psl
# to add in application makefile:
#ext_libs := expat curl
#ext_libs := $(addprefix $(em_ext_libs_path)/lib/lib,$(addsuffix .a, $(ext_libs))) -lopenal

picking ?= ANALYTICAL
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

ifeq ($(findstring avr,$(cross_prefix)),avr)
djnn_libs := core base
CXXFLAGS += -I/Applications/Arduino.app/Contents/Java/hardware/tools/avr/avr -I/usr/local/include
#https://github.com/andysworkshop/avr-stl/releases
endif

ARFLAGS ?= -r -u

ifeq (g++,$(findstring g++,$(CXX)))
CXXFLAGS += -Wno-psabi #https://stackoverflow.com/a/48149400
endif

# ---------------------------------------
# debug, analysis

#CFLAGS += -fsanitize=thread -O1
#LDFLAGS += -fsanitize=thread

#CFLAGS += -fsanitize=address -O1
#LDFLAGS += -fsanitize=address

#CFLAGS += -fsanitize=memory -O1
#LDFLAGS += -fsanitize=memory

#CFLAGS += -fsanitize=undefined
#LDFLAGS += -fsanitize=undefined

# scan_build: static analizer
# 1. download the latest llvm (+9.0.0) release from your platform using "Pre-Built Binaries" on http://releases.llvm.org/download.html
# 2. launch the CLI on this Makefile wih: 
#	/path/to/llvm/bin/scan-build -o build/scan-report/htmldir make -j
# 3. vizualize report with CLI (this command is given at the end of the above command: 
#	/path/to/llvm/bin/scan-view build/scan-report/htmldir/xxxx-xx-xx-xxxxx-xxxxx-x 

tidy := /usr/local/Cellar/llvm/5.0.1/bin/clang-tidy
tidy_opts := -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk 

%_tidy: %
	$(tidy) -header-filter="djnn" -checks="*" -extra-arg=-std=c++14 $^ -- $(tidy_opts)
.PHONY: %_tidy

all_tidy := $(addsuffix _tidy,$(srcs))
tidy: $(all_tidy)


# ---------------------------------------
# djnn modules

CXXFLAGS += -std=c++14
CXXFLAGS += $(CFLAGS)

djnn_libs ?= core exec_env base display comms gui input animation utils files audio
djnn_libs += $(djnn_libs_extra)
#extra

ifeq ($(cross_prefix),em)
djnn_libs := core exec_env base display gui input animation utils files audio
# comms input
endif

ifeq ($(physics), ODE)
djnn_libs += physics
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
lib_cflags :=
lib_cppflags :=
lib_objs :=
lib_djnn_deps :=
lib_ldflags :=
lib_pkg :=
lib_pkgpath :=

-include $$(src_dir)/$1/djnn-lib.mk

# default
$1_c_srcs ?= $$(filter %.c,$$(lib_srcs))
$1_cpp_srcs ?= $$(filter %.cpp,$$(lib_srcs))
$1_srcs = $$($1_cpp_srcs) $$($1_c_srcs) 
$1_objs ?= $$($1_cpp_srcs:.cpp=.o) $$($1_c_srcs:.c=.o)
$1_objs := $$(addprefix $(build_dir)/, $$($1_objs))

$1_srcgens ?= $$(lib_srcgens)
$1_objs += $$(lib_objs)

# fix srcs
$1_srcs := $$($1_srcs)

$1_pkg_deps :=
$1_deps := $$($1_objs:.o=.d)
$1_libname := libdjnn-$1$$(lib_suffix)
$1_lib := $$(build_lib_dir)/$$($1_libname)
$1_libname_static := libdjnn-$1$$(lib_static_suffix)
$1_lib_static := $$(build_lib_dir)/$$($1_libname_static)
$1_lib_cflags := $$(lib_cflags)
$1_lib_cppflags := $$(lib_cppflags)
$1_lib_ldflags := $$(lib_ldflags)
$1_lib_pkg := $$(lib_pkg)

$1_gperf_srcs :=
$1_cov_gcno  := $$($1_objs:.o=.gcno)
$1_cov_gcda  := $$($1_objs:.o=.gcda)

ifneq ($$($1_lib_pkg),)
$1_lib_pkgpath = $$(subst $$() $$(),:,$$(lib_pkgpath))
$1_lib_cflags += $$(shell env PKG_CONFIG_PATH=$$(PKG_CONFIG_PATH):$$($1_lib_pkgpath) pkg-config --cflags $$($1_lib_pkg))
$1_lib_ldflags += $$(shell env PKG_CONFIG_PATH=$$(PKG_CONFIG_PATH):$$($1_lib_pkgpath) pkg-config --libs $$($1_lib_pkg))
endif

$1_lib_all_ldflags := $$($1_lib_ldflags)

ifeq ($(os),$(filter $(os),Darwin MinGW))
ifdef lib_djnn_deps
$1_djnn_deps := $$(addsuffix $$(lib_suffix),$$(addprefix $$(build_lib_dir)/libdjnn-,$$(lib_djnn_deps)))
$1_lib_all_ldflags += $$(addprefix -ldjnn-,$$(lib_djnn_deps))
#$1_lib_all_ldflags += $$(foreach lib,$$(lib_djnn_deps), $$(value $$(lib)_lib_ldflags))
endif
endif

ifneq ($(os),em)
$1_lib_rpath := -Wl,-rpath,$$($1_libname)
endif
ifeq ($(os), Darwin)
$1_lib_rpath := -Wl,-install_name,$$($1_libname),
endif

$$($1_objs): CXXFLAGS+=$$($1_lib_cppflags)
$$($1_objs): CFLAGS+=$$($1_lib_cflags)
$$($1_lib): LDFLAGS+=$$($1_lib_all_ldflags)
#$$($1_lib): LDFLAGS+=$$($1_lib_ldflags)
$$($1_lib): $$($1_djnn_deps)

$$($1_lib): $$($1_objs)
ifeq ($$V,max)
	$$(CXX) $(DYNLIB) -o $$@ $$($1_objs) $$(LDFLAGS) $$($1_lib_rpath)
else
	@$(call rule_message,linking,$$(stylized_target))
	@$$(CXX) $(DYNLIB) -o $$@ $$($1_objs) $$(LDFLAGS) $$($1_lib_rpath)
endif

$$($1_lib_static): $$($1_objs)
ifeq ($$V,max)
	$$(AR) $$(ARFLAGS) $$@ $$($1_objs)
	$$(RANLIB) $$@
else
	@$(call rule_message,linking,$$(stylized_target))
	@$$(AR) $$(ARFLAGS) $$@ $$($1_objs)
	@$$(RANLIB) $$@
endif

$1_size: $$($1_lib_static)
	@ls -l $$^
	@$$(SIZE) -t $$^

$1_tidy_srcs := $$(addsuffix _tidy,$$($1_srcs))
$$($1_tidy_srcs): tidy_opts+=$$($1_lib_cppflags)

$1_tidy: $$($1_tidy_srcs)

$1: dirs $$($1_lib)

$1_clean:
	rm -f $$($1_objs)

$1_dbg:
#	@echo $$($1_objs)
#	@echo $$($1_djnn_deps)
#	@echo $$($1_lib_ldflags)

srcs += $$($1_srcs)
srcgens += $$($1_srcgens)
objs += $$($1_objs)
deps += $$($1_deps)
libs += $$($1_lib)
libs_static += $$($1_lib_static)
cov  += $$($1_cov_gcno) $$($1_cov_gcda) $(lcov_file)

endef

$(foreach a,$(djnn_libs),$(eval $(call lib_makerule,$a)))

uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))
all_pkg := $(call uniq,$(foreach lib,$(djnn_libs), $(value $(lib)_lib_pkg)))

djnn: $(libs)
static: $(libs_static)

size: $(libs_static)
	ls -l $(libs_static)
	$(SIZE) -t $(libs_static)

strip:
	strip $(libs_static)


# ---------------------------------------
# rules

$(build_dir)/%.o: %.cpp
ifeq ($V,max)
	$(CXX) $(CXXFLAGS) -c $< -o $@
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(CXX) $(CXXFLAGS) -c $< -o $@
endif

$(build_dir)/%.o: %.c
ifeq ($V,max)
	$(CC) $(CFLAGS) -c $< -o $@
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(CC) $(CFLAGS) -c $< -o $@
endif

# for generated .cpp
$(build_dir)/%.o: $(build_dir)/%.cpp
ifeq ($V,max)
	$(CXX) $(CXXFLAGS) -c $< -o $@
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(CXX) $(CXXFLAGS) -c $< -o $@
endif

$(build_dir)/%.cpp $(build_dir)/%.hpp: %.y
ifeq ($V,max)
	$(YACC) -v -o $@ $<
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(YACC) -v -o $@ $<
endif

$(build_dir)/%.cpp: %.l
ifeq ($V,max)
	$(LEX) -o $@ $<
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(LEX) -o $@ $<
endif

-include $(deps)

# ---------------------------------------
# coverage tests

lcov_file ?= $(build_dir)/djnn_cov.info
lcov_output_dir ?= $(build_dir)/coverage_html


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


# ---------------------------------------
# clean

clean:
	rm -f $(deps) $(objs) $(libs) $(srcgens) $(cov)
	rm -rf $(lcov_output_dir) > /dev/null 2>&1 || true
	rmdir $(build_dir) > /dev/null 2>&1 || true

distclean clear:
	rm -rf $(build_dir)

dbg:
	@echo $(all_pkg)


# ---------------------------------------
# package config

ifeq ($(prefix),)
# dev install
djnn_install_prefix :=  $(abspath $(build_dir))
pkg_config_install_prefix := /usr/local
else
# pkg install (brew, deb, arch)
djnn_install_prefix := $(abspath $(prefix))
pkg_config_install_prefix := $(abspath $(prefix))
endif

pkgconfig_targets = djnn-cpp-dev.pc djnn-cpp.pc
pkgconfig_targets := $(addprefix $(build_dir)/, $(pkgconfig_targets))

pkgconf: $(pkgconfig_targets)

$(build_dir)/%.pc: %.pc.in
	@mkdir -p $(dir $@)
	@sed -e 's,@PREFIX@,$(djnn_install_prefix),; s,@MAJOR@,$(MAJOR),; s,@MINOR@,$(MINOR),; s,@MINOR2@,$(MINOR2),' $< > $@

#----------------------------------------
# install 

install_pkgconf: pkgconf
	test -d $(pkg_config_install_prefix)/lib/pkgconfig || mkdir -p $(pkg_config_install_prefix)/lib/pkgconfig	
ifeq ($(prefix),)
	install -m 644 $(build_dir)/djnn-cpp-dev.pc	$(pkg_config_install_prefix)/lib/pkgconfig
else
	install -m 644 $(build_dir)/djnn-cpp.pc	$(pkg_config_install_prefix)/lib/pkgconfig
endif


all_headers = $(shell find src -type f -name "*.h")
all_headers_no_src = $(patsubst src/%,%,$(all_headers))

all_libs_no_build_dir = $(patsubst $(build_dir)/lib/%,%,$(libs))

install_headers: $(addprefix $(djnn_install_prefix)/include/djnn-cpp/,$(all_headers_no_src))

install_libs: $(addprefix $(djnn_install_prefix)/lib/,$(all_libs_no_build_dir))

$(djnn_install_prefix)/include/djnn-cpp/%.h: src/%.h
	@mkdir -p $(dir $@)
	install -m 644 $< $@

$(djnn_install_prefix)/lib/%$(lib_suffix): $(build_dir)/lib/%$(lib_suffix)
	@mkdir -p $(dir $@)
ifneq ($(prefix),)
	install -m 644 $< $@
endif

install: all install_pkgconf install_headers install_libs

install_clear:
	rm -rf $(djnn_install_prefix)

#----------------------------------------
# package builder

#deb

#note: 
# use dpkg-depcheck -d make to find out all dependency on djnn
# last tryon ubuntu 18_04: 
#      	qtbase5-dev-tools, pkg-config, bison, qtchooser, flex, m4
# install with:
#		sudo dpkg -i djnn-cpp_x.x.x.deb
# remove with:
#		sudo dpkg -r djnn-cpp
deb_prefix_version = build/deb/djnn-cpp_$(MAJOR).$(MINOR).$(MINOR2)
deb_prefix = $(deb_prefix_version)/usr
deb:	
	make -j6  install prefix=$(deb_prefix)
	test -d $(deb_prefix_version)/DEBIAN || mkdir -p $(deb_prefix_version)/DEBIAN
	sed -e 's,@PREFIX@,$(djnn_install_prefix),; s,@MAJOR@,$(MAJOR),; s,@MINOR@,$(MINOR),; s,@MINOR2@,$(MINOR2),' deb/control > $(deb_prefix_version)/DEBIAN/control
# remove debug symbol from library
	cd $(deb_prefix)/lib ; strip --strip-debug --strip-unneeded *.so
# remove rpath from library
	cd $(deb_prefix)/lib ; chrpath -d *.so
# build package with fakeroot
	cd "build/deb" ; fakeroot dpkg-deb --build djnn-cpp_$(MAJOR).$(MINOR).$(MINOR2)
# check integrity of the build package. We still have error
#	cd "build/deb" ; lintian djnn-cpp_$(MAJOR).$(MINOR).$(MINOR2).deb
.PHONY: deb


# ---------------------------------------
# package dependency installation

pkgdeps += bison flex

ifeq ($(os),Linux)
	pkgcmd := apt install -y
	pkgupg := apt upgrade -y

	pkgdeps += libexpat1-dev libcurl4-openssl-dev libudev-dev gperf libboost-thread-dev libevdev-dev libopenal-dev librtmidi-dev #libboost-fiber-dev
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
	pkgdeps += libusb
	pkgdeps += rtmidi
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
	mgwpkgdeps += freetype SDL2 SDL2_image cairo pango fontconfig libusb
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
