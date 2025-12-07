local_dir = $(src_dir)/gui/qt
my_srcs := $(wildcard $(local_dir)/*.cpp)

# -- specific precompiled header
# no gain :-/ ?!

# lib_pch_file := $(local_dir)/gui_precompiled.hpp
# lib_pch_dep_files := \
# 	qt_backend.o \
# 	qt_backend_shapes.o \
# 	qt_backend_style.o \
# 	qt_backend_transformation.o \
# 	qt_backend_text.o \
# 	qt_backend_text.o \
# 	qt/qqt_window.o qt/qqt_display.o qt/qt_picking_view.o qt/qqt_display.o
# lib_pch_dep_files := $(addprefix $(build_dir)/$(local_dir)/,$(lib_pch_dep_files))

# this will change local_dir !
include $(local_dir)/qt/djnn-lib-srcs.mk

qt_gui_libs := Qt$(qt_version)OpenGL Qt$(qt_version)Gui
ifeq ($(qt_version),6)
qt_gui_libs += Qt$(qt_version)OpenGLWidgets
endif

$(patsubst %.cpp,$(build_dir)/%.o,$(my_srcs)) \
$(build_dir)/$(local_dir)/qqt_window_moc.o \
: DJNN_CXXFLAGS += $(shell env PKG_CONFIG_PATH=$(display_lib_pkgpath) pkg-config --cflags $(qt_gui_libs))
# qt6: add Qt$(qt_version)OpenGLWidgets


lib_srcs += $(my_srcs) 

# qt_srcs := $(my_srcs)

# moncul:
# 	@echo $(qt_srcs)