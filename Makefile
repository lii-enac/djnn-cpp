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

DJNN_CXXFLAGS := $(CXXFLAGS)

PKG_CONFIG ?= pkg-config

# ---------------------------------------
# default

default: all

#all: config.mk djnn pkgconf # does not work anymore
all: djnn pkgconf
#dirs
#rules
#cccmd

help:
	@echo "default: djnn ; all: djnn"
	@echo "experiment make -j !!"

config config.mk:
# 	cp config.default.mk config.mk
	@echo "\ndefault config : QT"
	cp project/config/config_qt.mk config.mk

config_%:
	cp project/config/$@.mk config.mk

MAJOR = 1
MINOR = 21
MINOR2 = 0

-include config.mk
include project/config/config_qt.mk


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

# join https://stackoverflow.com/a/9551487
space :=
space +=
join-with = $(subst $(space),$1,$(strip $2))


# ---------------------------------------

all_srcs = $(call rwildcard,src/,*)
all_dirs = $(call uniq,$(dir $(all_srcs)))

# ---------------------------------------
# directory structure

#$(build_dir)/%/: %/
#	@mkdir -p $@

$(build_dir)/lib:
	@mkdir -p $@

$(build_dir)/include/djnn-cpp:
	@mkdir -p $@

dirs: $(build_dir)/lib $(build_dir)/include/djnn-cpp $(addprefix $(build_dir)/,$(all_dirs)) 


# ---------------------------------------
# stylized actions

# Set silent_opt to 's' if --quiet/-s set, otherwise ''.
silent_opt := $(filter s,$(word 1, $(MAKEFLAGS)))

silent =
ifeq ($(silent_opt),s)
silent = yes
endif
ifeq ($V,0)
silent = yes
endif

ifeq ($(silent),yes)
rule_message =
else
rule_message = printf "$1 $2\n"
endif

ifeq ($V,)
stylized_target = "\\033[1m$(notdir $@)\\033[0m"
else ifeq ($V,0)
#stylized_target = "\\033[1m$(notdir $@)\\033[0m"
else ifeq ($V,1)
stylized_target = "\\033[1m$(notdir $@)\\033[0m"
else ifeq ($V,2)
stylized_target = "\\033[1m$(notdir $@)\\033[0m" (older than $(notdir $?))
else ifeq ($V,3)
stylized_target = "\\033[1m$@\\033[0m" (older than $?)
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
ifeq ($(findstring MINGW,$(os)),MINGW)
os := MinGW
endif

# ifndef arch
# arch := $(shell uname -m)
# endif

build_lib_dir := $(build_dir)/lib
build_incl_dir := $(build_dir)/include/djnn-cpp
lib_static_suffix = .a

include_pch = -pch_include $(dst_pch)
echo ?= echo -e

CFLAGS += -MMD
CFLAGS += -Wall
CFLAGS += -Wextra -pedantic
CFLAGS += -Wno-unused-parameter #-Wno-vla-extension

CFLAGS += $(PRE_COV_CFLAGS)
LDFLAGS += $(PRE_COV_LDFLAGS)

ARFLAGS ?= rc

# STL
# DJNN_CXXFLAGS += -DDJNN_STL_EASTL
# DJNN_CXXFLAGS += -I/Users/conversy/recherche/istar/code/misc/EASTL/include -I/Users/conversy/recherche/istar/code/misc/EASTL/test/packages/EABase/include/Common/
# LDFLAGS += -L/Users/conversy/recherche/istar/code/misc/EASTL/build -lEASTL

CFLAGS += -I$(src_dir)
LDFLAGS += -L$(build_lib_dir)

ifeq ($(os),Linux)
CFLAGS += -fPIC
#DJNN_CXXFLAGS += -Wno-psabi #https://stackoverflow.com/a/48149400
lib_suffix =.so
DYNLIB ?= -shared
YACC ?= bison -d -Wno-conflicts-sr -Wno-conflicts-rr
thread = STD
moc := moc
compiler ?= gnu
endif

ifeq ($(os),Darwin)
ifeq ($(PREFIX),)
brew_prefix := $(shell brew --prefix)
else
brew_prefix := $(HOMEBREW_PREFIX)
endif
#CFLAGS += -Wno-deprecated-declarations
#CFLAGS += -isysroot $(shell xcode-select -p)/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk
lib_suffix =.dylib
DYNLIB ?= -dynamiclib
echo ?= echo
ifeq ($(origin YACC), default)
YACC := $(brew_prefix)/opt/bison/bin/bison -d -Wno-conflicts-sr
endif
ifeq ($(origin LEX), default)
LEX := $(brew_prefix)/opt/flex/bin/flex
endif
moc :=  moc #/usr/local/opt/qt/bin/moc
#boost name demangle
#DJNN_CXXFLAGS += -I/usr/local/include
compiler ?= llvm
#AR ?= /usr/bin/ar
#ARFLAGS ?= -r
endif

ifeq ($(os),MinGW)
CFLAGS += -fPIC
CFLAGS += -D_USE_MATH_DEFINES # https://docs.microsoft.com/en-us/cpp/c-runtime-library/math-constants?view=vs-2019
#DJNN_CXXFLAGS += -Wno-psabi #https://stackoverflow.com/a/48149400
lib_suffix =.dll
DYNLIB ?= -shared
YACC = bison -d -Wno-conflicts-sr -Wno-conflicts-rr
moc := moc
thread = STD
compiler ?= gnu
endif

ifeq ($(os),crazyflie)
CFLAGS += -Os
CFLAGS += -mfp16-format=ieee -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -ffunction-sections -fdata-sections
DJNN_CXXFLAGS += -Wno-psabi #https://stackoverflow.com/a/48149400
#boost name demangle
DJNN_CXXFLAGS += -I/usr/local/include
# djnn
DJNN_CXXFLAGS += -DDJNN_CRAZYFLIE -DDJNN_USE_FREERTOS
DJNN_CXXFLAGS += -DDJNN_NO_DEBUG -DDJNN_NO_SERIALIZE
DJNN_CXXFLAGS += -DDJNN_USE_STD_THREAD=1
DJNN_CXXFLAGS += --rtti #--rtti_data
DJNN_CXXFLAGS += -DRMT_ENABLED=0

lib_suffix =.so
DYNLIB = -shared
echo = echo
ARFLAGS = r
endif

ifeq ($(cross_prefix),em)
os := em
DYNLIB = -shared -r
lib_suffix=.bc
compiler ?= llvm

EMFLAGS := -Wall -Wno-unused-variable \
-s USE_BOOST_HEADERS -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_FREETYPE=1 -s USE_WEBGL2=1 \
-DSDL_DISABLE_IMMINTRIN_H \
-s DISABLE_EXCEPTION_CATCHING=0 \
-s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1 \
-s ASSERTIONS=2 \
-s ERROR_ON_UNDEFINED_SYMBOLS=0

#-s EXPORT_ALL=1 
#-s STANDALONE_WASM
EMFLAGS += -Oz

em_ext_libs_path ?= ../djnn-emscripten-ext-libs

EMCFLAGS += $(EMFLAGS) -I$(em_ext_libs_path)/include -I/usr/local/include #glm
CFLAGS += $(EMCFLAGS)
DJNN_CXXFLAGS += $(EMCFLAGS)
LDFLAGS += $(EMFLAGS)
# --emrun
#   $(ext_libs) # to add in application makefile
##idn2 expat curl fontconfig unistring psl
# to add in application makefile:
#ext_libs := expat curl
#ext_libs := $(addprefix $(em_ext_libs_path)/lib/lib,$(addsuffix .a, $(ext_libs))) -lopenal

picking ?= ANALYTICAL

all_libs = $(addprefix $(build_lib_dir)/,$(all_libs_no_build_dir))

#js wasm: $(build_lib_dir)/djnn.js $(src_dir)/c_api/djnn_js_api.js
#$(build_lib_dir)/djnn.js:
js: $(all_libs) $(src_dir)/c_api/djnn_js_api.js
	emcc $(EMFLAGS) -s "EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']" -s "EXPORTED_FUNCTIONS=`cat $(src_dir)/c_api/exported_functions.txt`" -o $(build_lib_dir)/djnn.js $(all_libs)
	cp $(src_dir)/c_api/djnn_js_api.js $(build_lib_dir)

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
DJNN_CXXFLAGS += -I/Applications/Arduino.app/Contents/Java/hardware/tools/avr/avr -I/usr/local/include
#https://github.com/andysworkshop/avr-stl/releases
endif

#ARFLAGS ?= -r -u


# compiler-specific options

ifeq ($(compiler),gcc)
#CFLAGS += -Wno-psabi #https://stackoverflow.com/a/48149400
endif

ifeq ($(compiler),llvm)
CFLAGS += -Wno-vla-cxx-extension
endif

linker ?= $(compiler)

ifeq ($(linker),mold)
# ifeq ($(os),Darwin)
# CXXLD := ld64.mold
# DJNN_CXXFLAGS += -fPIC
# LDFLAGS += -L/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/lib/
# LDFLAGS += -dylib -lc++ -lc
ifeq ($(compiler),gcc)
CXXLD ?= $(CXX) --use-ld=mold
endif
ifeq ($(compiler),llvm)
CXXLD ?= $(CXX) -fuse-ld=mold
endif
endif

# ---------------------------------------
# debug, analysis

CFLAGS += $(remotery_cflags)

#CFLAGS += -fsanitize=thread -O1
#LDFLAGS += -fsanitize=thread

#CFLAGS += -fsanitize=address -O1
#LDFLAGS += -fsanitize=address
#-fsanitize=leak

#CFLAGS += -fsanitize=memory -O1
#LDFLAGS += -fsanitize=memory

#CFLAGS += -fsanitize=undefined
#LDFLAGS += -fsanitize=undefined

# scan_build: static analyzer
# 1. download the latest llvm (+14.0.0) release from your platform (brew, apt) or using "Pre-Built Binaries" on http://releases.llvm.org/download.html
#     - Linux(deb): sudo apt install llvm-14 clang-tools-14
# 2. launch the CLI on this Makefile wih: 
#	/path/to/llvm/bin/scan-build -o build/scan-report/htmldir make -j
#      - Linux(deb) : /usr/lib/llvm-14/bin/scan-build
# 3. vizualize report with CLI (this command is given at the end of the above command: 
#	/path/to/llvm/bin/scan-view build/scan-report/htmldir/xxxx-xx-xx-xxxxx-xxxxx-x 

tidy := /usr/local/Cellar/llvm/5.0.1/bin/clang-tidy
tidy_opts := -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk 

%_tidy: %
	$(tidy) -header-filter="djnn" -checks="*" -extra-arg=-std=c++20 $^ -- $(tidy_opts)

all_tidy := $(addsuffix _tidy,$(srcs))
tidy: $(all_tidy)

# ---------------------------------------
# CFLAGS DJNN_CXXFLAGS

DJNN_CXXFLAGS += -std=c++17
DJNN_CXXFLAGS += $(CFLAGS)

# ---------------------------------------
# precompiled headers
use_pch ?= yes
ifeq ($(use_pch),yes)
# https://stackoverflow.com/questions/58841/precompiled-headers-with-gcc
# https://stackoverflow.com/questions/26755219/how-to-use-pch-with-clang

ifeq ($(compiler),llvm)
pch_ext ?= .pch
endif
ifeq ($(compiler),gnu)
pch_ext ?= .gch
endif

pch_file ?= $(src_dir)/core/utils/build/precompiled.hpp
pch_dst := $(build_dir)/$(pch_file)

ifeq ($(compiler),llvm)
DJNN_CXXFLAGS_PCH_INC += -include-pch $(pch_dst)$(pch_ext)
DJNN_CXXFLAGS_PCH_GEN += -fpch-instantiate-templates -fpch-codegen -fpch-debuginfo
pch_shared_dst := $(pch_dst:.hpp=.o)
endif
ifeq ($(compiler),gnu)
# https://stackoverflow.com/a/3164874
DJNN_CXXFLAGS_PCH_INC += -I$(dir $(pch_dst)) -include $(notdir $(pch_file)) -Winvalid-pch
#-fno-implicit-templates
#$(build_dir)/src/core/utils/build/external_template.o: DJNN_CXXFLAGS += -fimplicit-templates

# SDL and other stuff define new variables for compiling, canceling the use of pch with gnu cc
# FIXME this is not safe as every other external lib may define something
# https://gitlab.gnome.org/GNOME/gnome-online-accounts/-/merge_requests/14
# Both GCC and Clang appear to expand -pthread to define _REENTRANT on their own
# DJNN_CXXFLAGS_PCH_DEF += -D_REENTRANT
ifeq ($(display),SDL)
DJNN_CXXFLAGS_PCH_DEF += -Dmain=SDL_main
endif
endif

DJNN_CXXFLAGS_PCH := $(DJNN_CXXFLAGS)

$(build_dir)/%$(pch_ext): %
	@mkdir -p $(dir $@)
ifeq ($V,max)
	$(CXX) -x c++-header $(DJNN_CXXFLAGS) $(DJNN_CXXFLAGS_PCH_GEN) $< -o $@
else
	@$(call rule_message,compiling to,$(stylized_target))
	@$(CXX) -x c++-header $(DJNN_CXXFLAGS) $(DJNN_CXXFLAGS_PCH_GEN) $< -o $@
endif

$(build_dir)/%$(pch_ext): override DJNN_CXXFLAGS = $(DJNN_CXXFLAGS_PCH) $(DJNN_CXXFLAGS_CFG) $(DJNN_CXXFLAGS_PCH_DEF) $(djnn_cflags) $(DJNN_CXXFLAGS_COMMON) $(DJNN_CXXFLAGS_CK)

# for llvm -fpch-instantiate-templates -fpch-codegen
$(build_dir)/%precompiled.o: $(build_dir)/%precompiled.hpp$(pch_ext)
ifeq ($V,max)
	$(CXX) -c $< -o $@
	@printf "{\"directory\": \"$(root_dir)\", \"command\": \"$(CXX)  -c $< -o $@\", \"file\": \"$<\"}" > $(build_dir)/$*precompiled.cccmd.json
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(CXX) -c $< -o $@
	@printf "{\"directory\": \"$(root_dir)\", \"command\": \"$(CXX)  -c $< -o $@\", \"file\": \"$<\"}" > $(build_dir)/$*precompiled.cccmd.json
endif

pch: $(pch_dst)$(pch_ext)
clean_pch:
	rm -f $(pch_dst)$(pch_ext)

DJNN_CXXFLAGS += $(DJNN_CXXFLAGS_PCH_INC)

endif

# ---------------------------------------
# djnn modules


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


# ---------------------------------
# functions

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
lib_rules :=
lib_pch_file :=
lib_pch_dep_files :=
#define lib_rules
#endef

$1_libname := libdjnn-$1$$(lib_suffix)
$1_lib := $$(build_lib_dir)/$$($1_libname)
$1_libname_static := libdjnn-$1$$(lib_static_suffix)
$1_lib_static := $$(build_lib_dir)/$$($1_libname_static)

include $$(src_dir)/$1/djnn-lib.mk

# default
$1_c_srcs ?= $$(filter %.c,$$(lib_srcs))
$1_cpp_srcs ?= $$(filter %.cpp,$$(lib_srcs))
$1_srcs = $$($1_cpp_srcs) $$($1_c_srcs) 
$1_objs += $$($1_cpp_srcs:.cpp=.o) $$($1_c_srcs:.c=.o)
$1_objs := $$(addprefix $(build_dir)/, $$($1_objs))
ifeq ($$(compiler),llvm)
$1_objs += $$(pch_shared_dst)
endif

$1_srcgens ?= $$(lib_srcgens)
$1_objs += $$(lib_objs)

# freeze
$1_srcs := $$($1_srcs)
$1_lib_rules := $$(lib_rules)

$1_pkg_deps :=
$1_deps := $$($1_objs:.o=.d)

$1_lib_cflags := $$(lib_cflags)
$1_lib_cppflags := $$(lib_cppflags)
$1_lib_ldflags := $$(lib_ldflags)
$1_lib_pkg := $$(lib_pkg)

# -- specific precompiled header

$1_pch_file := $$(lib_pch_file)
$1_pch_dep_files := $$(lib_pch_dep_files)

ifneq ($$($1_pch_file),)
$1_pch_dest := $$(build_dir)/$$($1_pch_file)$$(pch_ext)
$$($1_pch_dep_files): $$($1_pch_dest)
$$($1_pch_dep_files): pch_file=$$($1_pch_file)
$$($1_pch_dep_files): DJNN_CXXFLAGS_PCH_GEN+=$$($1_lib_cflags)
endif

ifeq ($$(compiler),llvm)

ifneq ($$($1_pch_file),)
$1_pch_shared_dst := $$(build_dir)/$$($1_pch_file:.hpp=.o)
#$$($1_pch_shared_dst): $$($1_pch_dest)

$1_objs += $$($1_pch_shared_dst)
$$($1_lib): $$($1_pch_shared_dst)
endif

endif
	

$1_gperf_srcs :=
$1_cov_gcno  := $$($1_objs:.o=.gcno)
$1_cov_gcda  := $$($1_objs:.o=.gcda)

ifneq ($$($1_lib_pkg),)
$1_lib_pkgpath := $$(subst $$() $$(),:,$$(lib_pkgpath))
$1_lib_cflags += $$(shell PKG_CONFIG_PATH=$$(PKG_CONFIG_PATH):$$($1_lib_pkgpath) $(PKG_CONFIG) --cflags $$($1_lib_pkg))
$1_lib_ldflags += $$(shell PKG_CONFIG_PATH=$$(PKG_CONFIG_PATH):$$($1_lib_pkgpath) $(PKG_CONFIG) --libs $$($1_lib_pkg))
$1_lib_ldflags_static += $$(shell PKG_CONFIG_PATH=$$(PKG_CONFIG_PATH):$$($1_lib_pkgpath) $(PKG_CONFIG) --libs --static $$($1_lib_pkg))
endif

$1_lib_all_ldflags := $$($1_lib_ldflags)
$1_lib_all_ldflags_static := $$($1_lib_ldflags_static)

ifeq ($(os),$(filter $(os),Darwin Linux MinGW))
ifdef lib_djnn_deps
$1_djnn_deps := $$(addsuffix $$(lib_suffix),$$(addprefix $$(build_lib_dir)/libdjnn-,$$(lib_djnn_deps)))
$1_lib_djnn_deps := $$(addprefix -ldjnn-,$$(lib_djnn_deps))
#$1_lib_all_ldflags += $$1_lib_djnn_deps
#$1_lib_all_ldflags += $$(foreach lib,$$(lib_djnn_deps), $$(value $$(lib)_lib_ldflags))
endif
endif

ifneq ($(os), em)
#$1_lib_soname := -Wl,-soname,$$($1_libname)
#$1_lib_soname := -Wl,-rpath,$$(build_dir)

ifeq ($(linker), gnu)
$1_lib_soname += -Wl,-rpath-link,$$(abspath $$(build_dir))/lib
ifeq (-DRMT_USE_OPENGL=1,$(filter -DRMT_USE_OPENGL=1,$(remotery_cflags)))
$1_lib_soname += -Wl,-flat_namespace,-undefined,dynamic_lookup
else
$1_lib_soname += -Wl,--no-undefined
endif
endif

ifeq ($(linker), llvm)
$1_lib_soname := -Wl,-install_name,$$(abspath $$(build_dir))/lib/$$($1_libname) -Wl,-current_version,1.0.0 -Wl,-compatibility_version,1.0.0 
#$1_lib_soname :=
endif
ifeq ($(linker), mold)
#$1_lib_soname := -Wl,-install_name,$$($1_libname)
endif
endif

# the remaining will be put into a .mk file for further, faster, inclusion

#define $1_mk_content
$$($1_objs): $(pch_dst)$(pch_ext)
$$($1_objs): DJNN_CXXFLAGS+=$$($1_lib_cppflags) $$($1_lib_cflags)
$$($1_objs): CFLAGS+=$$($1_lib_cflags)
$$($1_lib): LDFLAGS+=$$($1_lib_all_ldflags)
#$$($1_lib): LDFLAGS+=$$($1_lib_ldflags)
$$($1_lib): $$($1_djnn_deps)

CXXLD ?= $$(CXX)

$$($1_lib): $$($1_objs)
	@mkdir -p $$(dir $$@)
ifeq ($$V,max)
	$$(CXXLD) $(DYNLIB) -o $$@ $$($1_objs) $$(LDFLAGS) $$($1_lib_djnn_deps) $$($1_lib_soname) $(lib_soname)
else
	@$(call rule_message,linking,$$(stylized_target))
	@$$(CXXLD) $(DYNLIB) -o $$@ $$($1_objs) $$(LDFLAGS) $$($1_lib_djnn_deps) $$($1_lib_soname) $(lib_soname)
endif

$$($1_lib_static): $$($1_objs)
	@mkdir -p $$(dir $$@)
ifeq ($$V,max)
	$$(AR) $$(ARFLAGS) $$@ $$($1_objs)
	$$(RANLIB) $$@
else
	@$(call rule_message,linking,$$(stylized_target))
	@$$(AR) $$(ARFLAGS) $$@ $$($1_objs) >& /dev/null
	@$$(RANLIB) $$@
endif

$1_size: $$($1_lib_static)
	@ls -l $$^
	@$$(SIZE) -t $$^

$1_tidy_srcs := $$(addsuffix _tidy,$$($1_srcs))
$$($1_tidy_srcs): tidy_opts+=$$($1_lib_cppflags)

$1_tidy: $$($1_tidy_srcs)

$1: $$($1_lib)

$1_clean:
	rm -f $$($1_objs)

$1_dbg:
#	@echo $$($1)
#	@echo $$($1_objs)
	@echo $$($1_djnn_deps)
	@echo $$($1_lib_ldflags)

srcs += $$($1_srcs)
srcgens += $$($1_srcgens)
objs += $$($1_objs)
deps += $$($1_deps)
libs += $$($1_lib)
libs_static += $$($1_lib_static)
ldflags_static += $$($1_lib_all_ldflags_static) $$($1_lib_ldflags)
cov  += $$($1_cov_gcno) $$($1_cov_gcda) $(lcov_file)

#endef
#$$(eval $$(lib_rules)) # does not work since it introduces new rules

endef

# -----------------
# old scheme
$(foreach a,$(djnn_libs),$(eval $(call lib_makerule,$a)))

# -----------------
# scheme to avoid calling lib functions each time we do a 'make'
# including previous computed rules leads to much faster 'make' invocations (especially on msys2)

# define newline


# endef

# # build a rule for each lib. This will be called once on the first 'make' invocation
# define lib_include_rule
# $(build_dir)/src/$1/$1_rules.mk: $(src_dir)/$1/djnn-lib.mk
# 	@#echo building $(build_dir)/src/$1/$1_rules.mk
# 	$(eval $(call lib_makerule,$1))
# 	@mkdir -p $(build_dir)/src/$1
# 	@printf -- '$$(subst $$(newline),\n,$$($1_mk_content))' > $$@
# 	@printf -- '$$(subst $$(newline),\n,$$($1_lib_rules))' >> $$@

# -include $(build_dir)/src/$1/$1_rules.mk
# endef

# # all-including rule. This will be built only once on first 'make' invocation
# lib_rules_mk = $(build_dir)/src/lib_rules.mk

# # test if the all-including rule exists
# ifneq ($(wildcard $(lib_rules_mk)),)
# # file exists: no need to call functions, just include previous results. Fast.
# -include $(lib_rules_mk)

# else
# # file does not exist: call all functions and produce .mk files. Slow.
# $(lib_rules_mk): $(foreach a,$(djnn_libs),$(build_dir)/src/$a/$a_rules.mk)
# 	@#echo building $@
# 	@printf -- "$(foreach a,$(djnn_libs),-include $(build_dir)/src/$a/$a_rules.mk\n)" > $@
# $(lib_rules_mk): config.mk

# $(foreach a,$(djnn_libs),$(eval $(call lib_include_rule,$a)))
# endif

# rules: $(lib_rules_mk)

# # caveat: do 'make clean_rules' whenever you change a djnn-lib.mk
# clean_rules:
# 	rm -f $(lib_rules_mk)
# 	rm -f $(foreach a,$(djnn_libs),$(build_dir)/src/$a/$a_rules.mk)

# end of new scheme

djnn: $(libs)
static: $(libs_static)

# ---------------------------------------

size: $(libs_static)
	ls -l $(libs_static)
	$(SIZE) -t $(libs_static)

strip:
	strip $(libs_static)

# ---------------------------------------
# commands.json for linters in VS Code etc.
# should be here, after the call to lib_makerule

all_cccmd_json = $(objs:%.o=%.cccmd.json)
all_cccmd_json_joined = $(call join-with, join ,$(all_cccmd_json))

$(build_dir)/compile_commands.json: $(all_cccmd_json)
	@echo creating $(@)
	@echo "," > join
	@echo "[" > $(@)
	@cat $(all_cccmd_json_joined) >> $(@)
	@echo "]" >> $(@)
	@rm join

cccmd: $(build_dir)/compile_commands.json


# ---------------------------------------
# generate ninja file from Makefile

make2ninja ?= tools/make2ninja.py

ninja: clear build.ninja
build.ninja: $(make2ninja)
	$(MAKE) -Bnd V=max | python3 $(make2ninja) > build.ninja
.PHONY: build.ninja ninja

$(make2ninja):
	mkdir -p $(dir $@)
	curl -O https://raw.githubusercontent.com/conversy/make2ninja/master/make2ninja.py && mv make2ninja.py $(make2ninja)
ifeq ($(os),Darwin)
	sed -i '' "s/\'(./\`(./" $@
endif

# ---------------------------------------
# generic rules

# https://stackoverflow.com/a/23324703
root_dir := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

$(build_dir)/%.o: %.cpp
	@mkdir -p $(dir $@)
ifeq ($V,max)
	$(CXX) $(DJNN_CXXFLAGS) -c $< -o $@
	@printf "{\"directory\": \"$(root_dir)\", \"command\": \"$(CXX) $(DJNN_CXXFLAGS) -c $< -o $@\", \"file\": \"$<\"}" > $(build_dir)/$*.cccmd.json
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(CXX) $(DJNN_CXXFLAGS) -c $< -o $@
	@printf "{\"directory\": \"$(root_dir)\", \"command\": \"$(CXX) $(DJNN_CXXFLAGS) -c $< -o $@\", \"file\": \"$<\"}" > $(build_dir)/$*.cccmd.json
endif

$(build_dir)/%.o: %.c
	@mkdir -p $(dir $@)
ifeq ($V,max)
	$(CC) $(CFLAGS) -c $< -o $@
	@printf "{\"directory\": \"$(root_dir)\", \"command\": \"$(CC) $(CFLAGS) -c $< -o $@\", \"file\": \"$<\"}" > $(build_dir)/$*.cccmd.json
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "{\"directory\": \"$(root_dir)\", \"command\": \"$(CC) $(CFLAGS) -c $< -o $@\", \"file\": \"$<\"}" > $(build_dir)/$*.cccmd.json
endif

# for generated .cpp
$(build_dir)/%.o: $(build_dir)/%.cpp
	@mkdir -p $(dir $@)
ifeq ($V,max)
	$(CXX) $(DJNN_CXXFLAGS) -c $< -o $@
	@printf "{\"directory\": \"$(root_dir)\", \"command\": \"$(CXX) $(DJNN_CXXFLAGS) -c $< -o $@\", \"file\": \"$<\"}" > $(build_dir)/$*.cccmd.json
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(CXX) $(DJNN_CXXFLAGS) -c $< -o $@
	@printf "{\"directory\": \"$(root_dir)\", \"command\": \"$(CXX) $(DJNN_CXXFLAGS) -c $< -o $@\", \"file\": \"$<\"}" > $(build_dir)/$*.cccmd.json
endif

$(build_dir)/%.cpp $(build_dir)/%.hpp: %.y
	@mkdir -p $(dir $@)
ifeq ($V,max)
	$(YACC) -v -o $@ $<
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(YACC) -v -o $@ $<
endif

$(build_dir)/%.cpp: %.l
	@mkdir -p $(dir $@)
ifeq ($V,max)
	$(LEX) -o $@ $<
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(LEX) -o $@ $<
endif


#----------------------------------------
# dependencies

-include $(deps)


# ---------------------------------------
# coverage tests

lcov_file ?= $(build_dir)/djnn_cov.info

pre_cov: PRE_COV_CFLAGS += --coverage -O0
pre_cov: PRE_COV_LDFLAGS += --coverage
pre_cov: dirs djnn
	lcov -d $(build_dir) -b . --zerocounters

cov:
	lcov -o $(lcov_file) -c -d . -b . --no-external > /dev/null 2>&1
	# remove useless reference
	lcov --remove $(lcov_file) '*/css-parser/*.*' -o $(lcov_file)
	lcov --remove $(lcov_file) '*/qt/*_moc.cpp' -o $(lcov_file)


# ---------------------------------------
# clean

clean:
	rm -f $(deps) $(objs) $(libs) $(srcgens) $(cov)
	rmdir $(build_dir) > /dev/null 2>&1 || true

distclean clear:
	rm -rf $(build_dir)

# ---------------------------------------
# package config

ifeq ($(PREFIX),)
# dev install
djnn_install_prefix := $(abspath $(build_dir))
ifeq ($(os),Darwin)
pkg_config_install_prefix := /usr/local
else ifeq ($(os),vcpkg)
pkg_config_install_prefix := /Users/conversy/recherche/istar/code/misc/vcpkg/installed/arm64-osx/lib/pkgconfig
else
pkg_config_install_prefix := /usr
endif

else
# pkg install (brew, deb, arch)
djnn_install_prefix := $(abspath $(DESTDIR)$(PREFIX))
pkg_config_install_prefix := $(abspath $(DESTDIR)$(PREFIX))
ifeq ($(os),Linux)
sed_pkg_config_install_prefix := /usr
else
sed_pkg_config_install_prefix := pkg_config_install_prefix
endif

endif

djnn_install_headers_dir ?= $(djnn_install_prefix)/include/djnn-cpp
djnn_install_lib_dir ?= $(djnn_install_prefix)/lib

pkgconfig_targets = djnn-cpp-dev.pc djnn-cpp.pc
pkgconfig_targets := $(addprefix $(build_dir)/, $(pkgconfig_targets))

pkgconf: $(pkgconfig_targets)

$(build_dir)/%.pc: distrib/%.pc.in
	@mkdir -p $(dir $@)
	@sed -e 's,@PREFIX@,$(sed_pkg_config_install_prefix),; s,@LIBPRIVATE@,$(call uniq,$(filter-out -lssl -lcrypto,$(ldflags_static))),; s,@MAJOR@,$(MAJOR),; s,@MINOR@,$(MINOR),; s,@MINOR2@,$(MINOR2),' $< > $@


#----------------------------------------
# install 

install_pkgconf: pkgconf
	test -d $(pkg_config_install_prefix)/lib/pkgconfig || mkdir -p $(pkg_config_install_prefix)/lib/pkgconfig	
ifeq ($(PREFIX),)
	install -m 644 $(build_dir)/djnn-cpp-dev.pc	$(pkg_config_install_prefix)/lib/pkgconfig
else
	install -m 644 $(build_dir)/djnn-cpp.pc	$(pkg_config_install_prefix)/lib/pkgconfig
endif


all_headers = $(call rwildcard,src/,*.h) $(call rwildcard,src/,*.hpp)
all_headers_no_src = $(patsubst src/%,%,$(all_headers))

all_libs_no_build_dir = $(patsubst $(build_dir)/lib/%,%,$(libs))


install_headers: $(addprefix $(djnn_install_headers_dir)/,$(all_headers_no_src))
install_libs: $(addprefix $(djnn_install_lib_dir)/,$(all_libs_no_build_dir))

$(djnn_install_headers_dir)/%.h: src/%.h
	@mkdir -p $(dir $@)
	install -m 644 $< $@

$(djnn_install_prefix)/include/djnn-cpp/%.hpp: src/%.hpp
	@mkdir -p $(dir $@)
	install -m 644 $< $@

$(djnn_install_prefix)/lib/%$(lib_suffix): $(build_dir)/lib/%$(lib_suffix)
	@mkdir -p $(dir $@)
ifneq ($(PREFIX),)
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
# last try on ubuntu 18_04: 
#      	qtbase5-dev-tools, pkg-config, bison, qtchooser, flex, m4
# install with:
#		sudo dpkg -i djnn-cpp_x.x.x.deb
# remove with:
#		sudo dpkg -r djnn-cpp
deb_prefix_version = build/deb/djnn-cpp_$(MAJOR).$(MINOR).$(MINOR2)
deb_prefix = $(deb_prefix_version)/usr
deb:
	make -j6  install PREFIX=$(deb_prefix)
	test -d $(deb_prefix_version)/DEBIAN || mkdir -p $(deb_prefix_version)/DEBIAN
	sed -e 's,@PREFIX@,$(djnn_install_prefix),; s,@MAJOR@,$(MAJOR),; s,@MINOR@,$(MINOR),; s,@MINOR2@,$(MINOR2),' distrib/deb/control > $(deb_prefix_version)/DEBIAN/control
# cp triggers file
	cp distrib/deb/triggers $(deb_prefix_version)/DEBIAN/triggers
# remove debug symbol from library
	cd $(deb_prefix)/lib ; strip --strip-debug --strip-unneeded *.so
# remove rpath from library
#	cd $(deb_prefix)/lib ; chrpath -d *.so
# build package with fakeroot
	cd "build/deb" ; fakeroot dpkg-deb --build djnn-cpp_$(MAJOR).$(MINOR).$(MINOR2)
# check integrity of the build package. We still have error
#	cd "build/deb" ; lintian djnn-cpp_$(MAJOR).$(MINOR).$(MINOR2).deb


MINOR3 = $(shell echo `date +%j`)
deb_git_prefix_version = build/deb/djnn-cpp_$(MAJOR).$(MINOR).$(MINOR2).$(MINOR3)
deb_git_prefix = $(deb_git_prefix_version)/usr
deb_git:
	make -j6  install PREFIX=$(deb_git_prefix)
	test -d $(deb_git_prefix_version)/DEBIAN || mkdir -p $(deb_git_prefix_version)/DEBIAN
	sed -e 's,@PREFIX@,$(djnn_install_prefix),; s,@MAJOR@,$(MAJOR),; s,@MINOR@,$(MINOR),; s,@MINOR2@,$(MINOR2).$(MINOR3),' distrib/deb/control > $(deb_git_prefix_version)/DEBIAN/control
	cp distrib/deb/triggers $(deb_git_prefix_version)/DEBIAN/triggers
	cd $(deb_git_prefix)/lib ; strip --strip-debug --strip-unneeded *.so
	cd "build/deb" ; fakeroot dpkg-deb --build djnn-cpp_$(MAJOR).$(MINOR).$(MINOR2).$(MINOR3)

#pkg

#note: 
#      	makepkg
# install with:
#		pacman -U djnn-cpp.pkg 
# remove with:
#		pacman -Rs djnn-cpp
pkg_destdir_version = build/pkg/djnn-cpp_$(MAJOR).$(MINOR).$(MINOR2)
ifeq ($(PREFIX),)
pkg_prefix = /usr
else
pkg_prefix = $(PREFIX)
endif
ifeq ($(DESTDIR),) 
pkg_destdir= $(shell pwd)/$(pkg_destdir_version)
else
pkg_destdir = $(DESTDIR)
endif
pkg:
#   for test only	
#	make DESTDIR=$(pkg_destdir) PREFIX=$(pkg_prefix) install
	test -d build || mkdir -p build
	sed -e 's,@MAJOR@,$(MAJOR),; s,@MINOR@,$(MINOR),;	 s,@MINOR2@,$(MINOR2),' distrib/PKGBUILD.proto > build/PKGBUILD
	cd build ; makepkg -f --skipinteg 
.PHONY: pkg


# ---------------------------------------
# package dependency installation

# should use the following variable
# all_pkg = $(call uniq,$(foreach lib,$(djnn_libs), $(value $(lib)_lib_pkg)))


ifeq ($(pkgcmd),)

ifeq ($(os),Linux)
pkgcmd := $(shell which apt || which apk)
endif
ifeq ($(os),Darwin)
pkgcmd := brew
endif
ifeq ($(os),MinGW)
pkgcmd := pacman
endif

endif

ifneq ($(pkgcmd),)
pkgcmdtype := $(notdir $(pkgcmd))

ifeq ($(pkgcmdtype),apt)
	pkginst := apt install -y
	pkgupg := apt upgrade -y
endif

ifeq ($(pkgcmdtype),apk)
	pkginst := apk add
	pkgupg := apk upgrade
endif

ifeq ($(pkgcmdtype),brew)
	pkginst := brew install
	pkgupg := brew upgrade
endif

ifeq ($(pkgcmdtype),pacman)
	pkginst := pacman -S --needed
	pkgupg := pacman -Syu --needed
endif

#untested - source only :-/
# ifeq ($(pkgcmdtype),vcpkg)
# 	pkginst := vcpkg install
# 	pkgupg := apt
	
# 	pkgdeps += pkgconf
# 	#pkgdeps += llvm
# 	pkgdeps += expat curl
# 	pkgdeps += openal-soft-dev
# 	ifeq ($(display),QT)
# 	pkgdeps += qt5
# 	endif

# 	#CXX = tools/cccl
# endif

#untested - binaries, but unusual way of managing pkg :-/
# ifeq ($(pkgcmdtype),conan)
# 	pkginst := apt add
# 	pkgupg := apt
# endif

endif
#else

#pkgdeps += make # obvioulsy already installed
pkgdeps += bison flex

ifeq ($(pkgcmdtype),apt)
pkgdeps += pkg-config
pkgdeps += g++
pkglibdeps += udev1 rtmidi6
pkglibdeps += expat1 curl4 evdev2 openal1
#libboost-thread-dev 
#libboost-fiber-dev
ifeq ($(display),QT)
	pkglibdeps += qt5opengl5
	#pkgdeps += qt5-default #for ubuntu < 22.04
endif
ifeq ($(display),SDL)
	ifeq ($(specialtarget),raspberry-ua-netinst)
		# on rpi, compile and install sdl2 with KMSDRM support, raspbian's sdl2 does not provide it by default
		#pkgdeps += raspberrypi
	else
		pkglibdeps += sdl2-dev sdl2-image # gbm
	endif
endif
ifeq ($(display),DRM)
	pkglibdeps += drm
endif
ifeq ($(graphics),CAIRO)
	pkglibdeps += cairo pango1.0 #libpangocairo-1.0-0 
endif
ifeq ($(graphics),GL)
	pkglibdeps += freetype6 fontconfig1 glm
endif
ifeq ($(graphics),SDLGPU)
	pkglibdeps += freetype6 fontconfig1 glm
endif
pkglibdeps_full := $(addprefix lib,$(pkglibdeps))
#pkglibdeps_full := $(addsuffix -dev,$(pkglibdeps_full))
pkgdeps += $(pkglibdeps_full)
endif

toto:
	@echo $(pkglibdeps_full)

ifeq ($(pkgcmdtype),apk)
pkgdeps += pkgconfig
pkgdeps += g++
pkglibdeps += eudev rtmidi
pkglibdeps += curl libevdev expat openal-soft flex

ifeq ($(display),QT)
	pkglibdeps += qt5opengl5
endif
ifeq ($(display),SDL)
	ifeq ($(specialtarget),raspberry-ua-netinst)
		# on rpi, compile and install sdl2 with KMSDRM support, raspbian's sdl2 does not provide it by default
		#pkgdeps += raspberrypi
	else
		pkglibdeps += sdl2 sdl2_image # gbm-
	endif
endif
ifeq ($(display),DRM)
	pkglibdeps += drm
endif
ifeq ($(graphics),CAIRO)
	pkglibdeps += cairo pango1.0 #libpangocairo-1.0-0 
endif
ifeq ($(graphics),GL)
	pkglibdeps += freetype fontconfig glm 
endif
ifeq ($(graphics),SDLGPU)
	pkglibdeps += freetype fontconfig glm
endif
#pkglibdeps_full := $(addsuffix -dev,$(pkglibdeps))
pkglibdeps_full := $(pkglibdeps)
pkgdeps += $(pkglibdeps_full)
endif

ifeq ($(pkgcmdtype),brew)
pkgdeps += pkg-config
pkgdeps += expat curl
#pkgdeps += libusb #crazyflie
pkgdeps += rtmidi
ifeq ($(graphics),QT)
	pkgdeps += qt5
endif
ifeq ($(display),SDL)
	#pkgdeps += sdl2 sdl2_image
	pkgdeps += sdl3
	#sdl3_image
endif
ifeq ($(graphics),CAIRO)
	pkgdeps += cairo pango
endif
ifeq ($(graphics),GL)
	pkgdeps += glm fontconfig freetype2
endif
ifeq ($(graphics),SDLGPU)
	pkgdeps += glm fontconfig freetype2
endif
ifeq ($(audio),$(filter $(audio),AL AL_SOFT))
	pkgdeps += openal-soft
endif
endif

ifeq ($(pkgcmdtype),pacman)
	#https://www.msys2.org/

	#pkgdeps := git make pkg-config

	#pkginst := pacboy -S --needed
	#pkgupg := pacboy -Syu --needed

	#boost
	mgwpkgdeps += gcc expat curl
	#pkgdeps += libusb #crazyflie
	mgwpkgdeps += rtmidi
	ifeq ($(graphics),QT)
		mgwpkgdeps += qt5-3d
	endif
	ifeq ($(display),SDL)
		mgwpkgdeps += SDL2 SDL2_image
	endif
	ifeq ($(graphics),CAIRO)
		mgwpkgdeps += cairo pango
	endif
	ifeq ($(graphics),GL)
		mgwpkgdeps += glm fontconfig freetype
	endif
	ifeq ($(graphics),SDLGPU)
		mgwpkgdeps += glm fontconfig freetype
	endif
	ifeq ($(audio),$(filter $(audio),AL AL_SOFT))
		mgwpkgdeps += openal
	endif

	# mgwpkgdeps += gcc boost expat curl qt5
	# mgwpkgdeps += freetype SDL2 SDL2_image cairo pango fontconfig libusb
	# ifeq ($(graphics),GL)
	# 	mgwpkgdeps += glm
	# endif
	mgwpkgdeps := $(addprefix mingw-w64-x86_64-, $(mgwpkgdeps))
	pkgdeps += $(mgwpkgdeps)
endif

#endif

# ifeq ($(os),vcpkg)
# 	#https://www.msys2.org/

# 	pkginst := /Users/conversy/recherche/istar/code/misc/vcpkg/vcpkg
# 	pkgupg := /Users/conversy/recherche/istar/code/misc/vcpkg/vcpkg

# 	mgwpkgdeps += gcc
# 	mgwpkgdeps += expat curl
# 	#mgwpkgdeps += rtmidi

# 	ifeq ($(graphics),QT)
# 		mgwpkgdeps += qt5
# 	endif
# 	ifeq ($(display),SDL)
# 		mgwpkgdeps += SDL2 SDL2_image
# 	endif
# 	ifeq ($(graphics),CAIRO)
# 		mgwpkgdeps += cairo pango
# 	endif
# 	ifeq ($(graphics),GL)
# 		mgwpkgdeps += glm fontconfig freetype
# 	endif
# 	ifeq ($(graphics),SDLGPU)
# 		mgwpkgdeps += glm fontconfig freetype
# 	endif
# 	ifeq ($(audio),$(filter $(audio),AL AL_SOFT))
# 		mgwpkgdeps += openal
# 	endif

# 	#mgwpkgdeps := $(addprefix mingw-w64-x86_64-, $(mgwpkgdeps))
# 	pkgdeps += $(mgwpkgdeps)
# endif

install-pkgdeps:
	$(pkginst) $(pkgdeps)

upgrade-pkgdeps:
	$(pkgupg) $(pkgdeps)

#-----------------------
# end of the ultimate Makefile
