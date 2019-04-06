local_dir := src/gui/qt/qt

$(build_dir)/$(local_dir)/moc_MyQQWindow.cpp: $(local_dir)/my_qqwindow.h
	$(moc) $< > $@

lib_objs += $(build_dir)/$(local_dir)/moc_MyQQWindow.o
lib_srcgens += $(build_dir)/$(local_dir)/moc_MyQQWindow.cpp
#lib_srcs += $(shell find $(local_dir) -name "*.cpp")
