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

all: config.mk dirs djnn pkgconf
#rules
#cccmd

help:
	@echo "default: djnn ; all: djnn"
	@echo "experiment make -j !!"

config.mk:
	cp config.default.mk config.mk

MAJOR = 1
MINOR = 16
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

# join https://stackoverflow.com/a/9551487
space :=
space +=
join-with = $(subst $(space),$1,$(strip $2))


# ---------------------------------------

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

CFLAGS += -MMD -Wall

CFLAGS += $(PRE_COV_CFLAGS)
LDFLAGS += $(PRE_COV_LDFLAGS)

# STL
# CXXFLAGS += -DDJNN_STL_EASTL
# CXXFLAGS += -I/Users/conversy/recherche/istar/code/misc/EASTL/include -I/Users/conversy/recherche/istar/code/misc/EASTL/test/packages/EABase/include/Common/
# LDFLAGS += -L/Users/conversy/recherche/istar/code/misc/EASTL/build -lEASTL

CFLAGS += -I$(src_dir)
LDFLAGS += -L$(build_lib_dir)

ifeq ($(os),Linux)
CFLAGS += -fpic
#CXXFLAGS += -Wno-psabi #https://stackoverflow.com/a/48149400
lib_suffix =.so
DYNLIB = -shared
YACC = bison -d
thread = STD
moc := moc
compiler ?= gnu
endif

ifeq ($(os),Darwin)
CFLAGS += -Wno-deprecated-declarations
lib_suffix =.dylib
DYNLIB = -dynamiclib
echo ?= echo
YACC := /usr/local/opt/bison/bin/bison -d
LEX := /usr/local/opt/flex/bin/flex
moc :=  moc #/usr/local/opt/qt/bin/moc
#boost name demangle
#CXXFLAGS += -I/usr/local/include
compiler ?= llvm
#AR ?= /usr/bin/ar
#ARFLAGS ?= -r
endif

ifeq ($(os),MinGW)
CFLAGS += -fpic
CFLAGS += -D_USE_MATH_DEFINES # https://docs.microsoft.com/en-us/cpp/c-runtime-library/math-constants?view=vs-2019
#CXXFLAGS += -Wno-psabi #https://stackoverflow.com/a/48149400
lib_suffix =.dll
DYNLIB = -shared
YACC = bison -d
moc := moc
thread = STD
compiler ?= gnu
endif

ifeq ($(os),crazyflie)
CFLAGS += -Os
CFLAGS += -mfp16-format=ieee -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -ffunction-sections -fdata-sections
CXXFLAGS += -Wno-psabi #https://stackoverflow.com/a/48149400
#boost name demangle
CXXFLAGS += -I/usr/local/include
# djnn
CXXFLAGS += -DDJNN_CRAZYFLIE -DDJNN_USE_FREERTOS
CXXFLAGS += -DDJNN_NO_DEBUG -DDJNN_NO_SERIALIZE
CXXFLAGS += -DDJNN_USE_STD_THREAD=1
CXXFLAGS += --rtti #--rtti_data
CXXFLAGS += -DRMT_ENABLED=0

lib_suffix =.so
DYNLIB = -shared
echo = echo
ARFLAGS = r
endif

ifeq ($(cross_prefix),em)
os := em
DYNLIB=
lib_suffix=.bc
compiler ?= llvm

EMFLAGS := -Wall -Wno-unused-variable \
-s USE_BOOST_HEADERS -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_FREETYPE=1 -s USE_WEBGL2=1 \
-DSDL_DISABLE_IMMINTRIN_H \
-s EXPORT_ALL=1 -s DISABLE_EXCEPTION_CATCHING=0 \
-s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1 \
-s ASSERTIONS=2 \
-s ERROR_ON_UNDEFINED_SYMBOLS=0

#-s STANDALONE_WASM
#EMFLAGS += -Oz

em_ext_libs_path ?= ../djnn-emscripten-ext-libs

EMCFLAGS += $(EMFLAGS) -I$(em_ext_libs_path)/include -I/usr/local/include #glm
CFLAGS += $(EMCFLAGS)
CXXFLAGS += $(EMCFLAGS)
LDFLAGS += $(EMFLAGS) --emrun
#   $(ext_libs) # to add in application makefile
##idn2 expat curl fontconfig unistring psl
# to add in application makefile:
#ext_libs := expat curl
#ext_libs := $(addprefix $(em_ext_libs_path)/lib/lib,$(addsuffix .a, $(ext_libs))) -lopenal

picking ?= ANALYTICAL

all_libs = $(addprefix $(build_lib_dir)/,$(all_libs_no_build_dir))

js wasm: $(build_lib_dir)/djnn.js
$(build_lib_dir)/djnn.js:
js:$(all_libs)
	emcc $(EMFLAGS) -s LINKABLE=1 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']" -o $(build_lib_dir)/djnn.js $(all_libs)

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

#ARFLAGS ?= -r -u

ifeq (g++,$(findstring g++,$(CXX)))
CXXFLAGS += -Wno-psabi #https://stackoverflow.com/a/48149400
endif

# ---------------------------------------
# debug, analysis

CFLAGS += $(remotery_cflags)

#CFLAGS += -fsanitize=thread -O1
#LDFLAGS += -fsanitize=thread

#CFLAGS += -fsanitize=address -O1
#LDFLAGS += -fsanitize=address -fsanitize=leak

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

all_tidy := $(addsuffix _tidy,$(srcs))
tidy: $(all_tidy)

# ---------------------------------------
# CFLAGS CXXFLAGS

CXXFLAGS += $(CFLAGS)
CXXFLAGS += -std=c++17

# ---------------------------------------
# precompiled headers

ifneq ($(use_pch),no)
# https://stackoverflow.com/questions/58841/precompiled-headers-with-gcc
# https://stackoverflow.com/questions/26755219/how-to-use-pch-with-clang

ifeq ($(compiler),llvm)
pch_ext = .pch
endif
ifeq ($(compiler),gnu)
pch_ext = .gch
endif

pch := precompiled.h
pch_src := $(src_dir)/core/utils/build/$(pch)
pch_dst := $(build_dir)/$(pch_src)$(pch_ext)

# SDL and other stuff define new variables for compiling, canceling the use of pch with gnu cc
# FIXME this is not safe as every other external lib may define something
ifeq ($(compiler),gnu)
# https://gitlab.gnome.org/GNOME/gnome-online-accounts/-/merge_requests/14
# Both GCC and Clang appear to expand -pthread to define _REENTRANT on their own
CXXFLAGS += -D_REENTRANT
ifeq ($(display),SDL)
CXXFLAGS += -Dmain=SDL_main
endif
endif

CXXFLAGS_PCH := $(CXXFLAGS)

$(pch_dst): $(pch_src)
ifeq ($V,max)
	$(CXX) -x c++-header $(CXXFLAGS_PCH) $< -o $@
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(CXX) -x c++-header $(CXXFLAGS_PCH) $< -o $@
endif

ifeq ($(compiler),llvm)
CXXFLAGS += -include-pch $(pch_dst)
#-fpch-instantiate-templates -fpch-codegen -fpch-debuginfo
endif
ifeq ($(compiler),gnu)
# https://stackoverflow.com/a/3164874
CXXFLAGS += -I$(dir $(pch_dst)) -include $(pch) -Winvalid-pch
#-fno-implicit-templates
#$(build_dir)/src/core/utils/build/external_template.o: CXXFLAGS += -fimplicit-templates
endif

endif

# ---------------------------------------
# djnn modules


djnn_libs ?= core exec_env base display comms gui input animation utils files audio
djnn_libs += $(djnn_libs_extra)
#extra

ifeq ($(cross_prefix),em)
djnn_libs := core exec_env base display gui input animation utils files audio c_api
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
#define lib_rules
#endef


include $$(src_dir)/$1/djnn-lib.mk

# default
$1_c_srcs ?= $$(filter %.c,$$(lib_srcs))
$1_cpp_srcs ?= $$(filter %.cpp,$$(lib_srcs))
$1_srcs = $$($1_cpp_srcs) $$($1_c_srcs) 
$1_objs ?= $$($1_cpp_srcs:.cpp=.o) $$($1_c_srcs:.c=.o)
$1_objs := $$(addprefix $(build_dir)/, $$($1_objs))

$1_srcgens ?= $$(lib_srcgens)
$1_objs += $$(lib_objs)

# freeze
$1_srcs := $$($1_srcs)
$1_lib_rules := $$(lib_rules)

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
$1_lib_soname := -Wl,-soname,$$($1_libname)
endif
ifeq ($(os), Darwin)
$1_lib_soname := -Wl,-install_name,$$($1_libname)
endif

# the remaining will be put into a .mk file for further, faster, inclusion

#define $1_mk_content
$$($1_objs): $(pch_dst)
$$($1_objs): CXXFLAGS+=$$($1_lib_cppflags)
$$($1_objs): CFLAGS+=$$($1_lib_cflags)
$$($1_lib): LDFLAGS+=$$($1_lib_all_ldflags)
#$$($1_lib): LDFLAGS+=$$($1_lib_ldflags)
$$($1_lib): $$($1_djnn_deps)

$$($1_lib): $$($1_objs)
ifeq ($$V,max)
	$$(CXX) $(DYNLIB) -o $$@ $$($1_objs) $$(LDFLAGS) $$($1_lib_soname)
else
	@$(call rule_message,linking,$$(stylized_target))
	@$$(CXX) $(DYNLIB) -o $$@ $$($1_objs) $$(LDFLAGS) $$($1_lib_soname)
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
	@echo $$($1)
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
static: dirs $(libs_static)

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
# generic rules

# https://stackoverflow.com/a/23324703
root_dir := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

$(build_dir)/%.o: %.cpp
ifeq ($V,max)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@printf "{\"directory\": \"$(root_dir)\", \"command\": \"$(CXX) $(CXXFLAGS) -c $< -o $@\", \"file\": \"$<\"}" > $(build_dir)/$*.cccmd.json
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@printf "{\"directory\": \"$(root_dir)\", \"command\": \"$(CXX) $(CXXFLAGS) -c $< -o $@\", \"file\": \"$<\"}" > $(build_dir)/$*.cccmd.json
endif

$(build_dir)/%.o: %.c
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
ifeq ($V,max)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@printf "{\"directory\": \"$(root_dir)\", \"command\": \"$(CXX) $(CXXFLAGS) -c $< -o $@\", \"file\": \"$<\"}" > $(build_dir)/$*.cccmd.json
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@printf "{\"directory\": \"$(root_dir)\", \"command\": \"$(CXX) $(CXXFLAGS) -c $< -o $@\", \"file\": \"$<\"}" > $(build_dir)/$*.cccmd.json
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


#----------------------------------------
# dependencies

-include $(deps)


# ---------------------------------------
# coverage tests

lcov_file ?= $(build_dir)/djnn_cov.info
lcov_output_dir ?= $(build_dir)/coverage_html

pre_cov: PRE_COV_CFLAGS += --coverage -O0
pre_cov: PRE_COV_LDFLAGS += --coverage
pre_cov: dirs djnn
	lcov -d $(build_dir) -b . --zerocounters

cov_jenkins:
	lcov -o $(lcov_file) -c -d . -b . --no-external > /dev/null 2>&1
	# remove .cpp from css-parser which are destroyed after compilation
	lcov --remove $(lcov_file) '*/css-parser/*.cpp' -o $(lcov_file)
	lcov --remove $(lcov_file) '*/qt/*_moc.cpp' -o $(lcov_file)
	# convert lcov report to gcov report
	# gcovr --xml-pretty -o $(build_dir)/gcov_report.xml -s
	genhtml -o $(lcov_output_dir) $(lcov_file)
cov: cov_jenkins
	cd $(lcov_output_dir) ; open index.html


# ---------------------------------------
# clean

clean:
	rm -f $(deps) $(objs) $(libs) $(srcgens) $(cov)
	rm -rf $(lcov_output_dir) > /dev/null 2>&1 || true
	rmdir $(build_dir) > /dev/null 2>&1 || true

distclean clear:
	rm -rf $(build_dir)

# ---------------------------------------
# package config

ifeq ($(PREFIX),)
# dev install
djnn_install_prefix :=  $(abspath $(build_dir))
ifeq ($(os),Darwin)
pkg_config_install_prefix := /usr/local
else
pkg_config_install_prefix := /usr
endif
else
# pkg install (brew, deb, arch)
djnn_install_prefix := $(abspath $(DESTDIR)$(PREFIX))
pkg_config_install_prefix := $(abspath $(DESTDIR)$(PREFIX))
endif

pkgconfig_targets = djnn-cpp-dev.pc djnn-cpp.pc
pkgconfig_targets := $(addprefix $(build_dir)/, $(pkgconfig_targets))

pkgconf: $(pkgconfig_targets)

$(build_dir)/%.pc: distrib/%.pc.in
	@mkdir -p $(dir $@)
	@sed -e 's,@PREFIX@,$(pkg_config_install_prefix),; s,@MAJOR@,$(MAJOR),; s,@MINOR@,$(MINOR),; s,@MINOR2@,$(MINOR2),' $< > $@


#----------------------------------------
# install 

install_pkgconf: pkgconf
	test -d $(pkg_config_install_prefix)/lib/pkgconfig || mkdir -p $(pkg_config_install_prefix)/lib/pkgconfig	
ifeq ($(PREFIX),)
	install -m 644 $(build_dir)/djnn-cpp-dev.pc	$(pkg_config_install_prefix)/lib/pkgconfig
else
	install -m 644 $(build_dir)/djnn-cpp.pc	$(pkg_config_install_prefix)/lib/pkgconfig
endif


all_headers = $(call rwildcard,src/,*.h)
all_headers_no_src = $(patsubst src/%,%,$(all_headers))

all_libs_no_build_dir = $(patsubst $(build_dir)/lib/%,%,$(libs))

install_headers: $(addprefix $(djnn_install_prefix)/include/djnn-cpp/,$(all_headers_no_src))

install_libs: $(addprefix $(djnn_install_prefix)/lib/,$(all_libs_no_build_dir))

$(djnn_install_prefix)/include/djnn-cpp/%.h: src/%.h
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
			pkgdeps += libsdl2-dev libsdl2-image-dev libgbm-dev
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

	#boost
	pkgdeps += expat curl
	#pkgdeps += libusb #crazyflie
	pkgdeps += rtmidi
	ifeq ($(graphics),QT)
		pkgdeps += qt5
	endif
	ifeq ($(display),SDL)
		pkgdeps += sdl2 sdl2_image
	endif
	ifeq ($(graphics),CAIRO)
		pkgdeps += cairo pango
	endif
	ifeq ($(graphics),GL)
		pkgdeps += glm fontconfig freetype2
	endif
	ifeq ($(audio),$(filter $(audio),AL AL_SOFT))
		pkgdeps += openal-soft
	endif
endif

ifeq ($(os),MinGW)
	#https://www.msys2.org/
	#pkgdeps := git make pkg-config

	#pkgcmd := pacboy -S --needed
	#pkgupg := pacboy -Syu --needed

	#boost
	mgwpkgdeps += expat curl
	#pkgdeps += libusb #crazyflie
	mgwpkgdeps += rtmidi
	ifeq ($(graphics),QT)
		mgwpkgdeps += qt5
	endif
	ifeq ($(display),SDL)
		mgwpkgdeps += SDL2 SDL2_image
	endif
	ifeq ($(graphics),CAIRO)
		mgwpkgdeps += cairo pango
	endif
	ifeq ($(graphics),GL)
		mgwpkgdeps += glm fontconfig freetype2
	endif
	ifeq ($(audio),$(filter $(audio),AL AL_SOFT))
		mgwpkgdeps += openal
	endif

	# pkgcmd := pacman -S --needed
	# pkgupg := pacman -Syu --needed

	pkgcmd := pacman -S --needed
	pkgupg := pacman -Syu --needed

	# mgwpkgdeps += gcc boost expat curl qt5
	# mgwpkgdeps += freetype SDL2 SDL2_image cairo pango fontconfig libusb
	# ifeq ($(graphics),GL)
	# 	mgwpkgdeps += glm
	# endif
	mgwpkgdeps := $(addprefix mingw-w64-x86_64-, $(mgwpkgdeps))
	pkgdeps += $(mgwpkgdeps)
endif

install-pkgdeps:
	$(pkgcmd) $(pkgdeps)

upgrade-pkgdeps:
	$(pkgupg) $(pkgdeps)

#-----------------------
# end of the ultimate Makefile
