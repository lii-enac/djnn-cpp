local_dir := src/gui/qt/qt

$(build_dir)/$(local_dir)/moc_MyQQWindow.cpp: $(local_dir)/my_qqwindow.h
ifeq ($V,max)
	$(moc) $< > $@
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(moc) $< > $@
endif

lib_objs += $(build_dir)/$(local_dir)/moc_MyQQWindow.o
lib_srcgens += $(build_dir)/$(local_dir)/moc_MyQQWindow.cpp
