local_dir := src/gui/qt

lib_srcs += $(shell find $(local_dir) -name "*.cpp")

#ifeq ($(graphics),QT)
include $(local_dir)/qt/djnn-lib-srcs.mk
#endif
