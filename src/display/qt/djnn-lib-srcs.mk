local_dir = $(src_dir)/display/qt

lib_srcs += $(call rwildcard,$(local_dir)/,*.cpp)

lib_objs += $(build_dir)/$(local_dir)/qt_window_moc.o
lib_srcgens += $(build_dir)/$(local_dir)/qt_window_moc.cpp

$(build_dir)/%_moc.cpp: %_moc.h
	@mkdir -p $(dir $@)
ifeq ($V,max)
	$(moc) $< > $@
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(moc) $< > $@
endif

$(build_dir)/$(local_dir)/qt_display.o \
$(build_dir)/$(local_dir)/qt_window.o \
$(build_dir)/$(local_dir)/qt_window_moc.o \
: DJNN_CXXFLAGS += $(shell env PKG_CONFIG_PATH=$(display_lib_pkgpath) pkg-config --cflags Qt5OpenGL Qt5Gui)

# -- specific precompiled header
# no gain :-/ ?!

# lib_pch_file := $(src_dir)/display/qt/display_precompiled.hpp
# lib_pch_dep_files := \
# 	qt_display.o \
# 	qt_window.o \
# 	qt_window_moc.o
# lib_pch_dep_files := $(addprefix $(build_dir)/$(local_dir)/,$(lib_pch_dep_files))


