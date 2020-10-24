local_dir := src/gui/qt
lib_srcs += $(wildcard $(local_dir)/*.cpp)

#ifeq ($(graphics),QT)
include $(local_dir)/qt/djnn-lib-srcs.mk
#endif
