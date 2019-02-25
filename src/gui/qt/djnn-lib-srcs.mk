$(build_dir)/src/gui/qt/moc_MyQWindow.cpp: src/gui/qt/my_qwindow.h src/gui/backend.h
	$(moc) $< > $@

lib_objs += $(build_dir)/src/gui/qt/moc_MyQWindow.o
lib_srcgens += $(build_dir)/src/gui/qt/moc_MyQWindow.cpp
lib_srcs += $(shell find src/gui/qt -name "*.cpp")

