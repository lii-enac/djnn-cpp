local_dir := $(src_dir)/gui/qt/qt
my_srcs += $(wildcard $(local_dir)/*.cpp)

lib_objs += $(build_dir)/$(local_dir)/qqt_window_moc.o
lib_srcgens += $(build_dir)/$(local_dir)/qqt_window_moc.cpp

ifeq ($(use_qopengl_widget),yes)
lib_cflags += -DUSE_QOPENGL_WIDGET=1
endif
