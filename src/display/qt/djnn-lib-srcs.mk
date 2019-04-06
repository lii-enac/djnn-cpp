local_dir := src/display/qt

$(build_dir)/$(local_dir)/moc_MyQWindow.cpp: $(local_dir)/my_qwindow.h
	$(moc) $< > $@

lib_objs += $(build_dir)/$(local_dir)/moc_MyQWindow.o
lib_srcgens += $(build_dir)/$(local_dir)/moc_MyQWindow.cpp
lib_srcs += $(shell find $(local_dir) -name "*.cpp")

