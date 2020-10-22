local_dir := src/gui/qt/qt

lib_objs += $(build_dir)/$(local_dir)/qqwindow_moc.o
lib_srcgens += $(build_dir)/$(local_dir)/qqwindow_moc.cpp

define my_lib_rules

$(build_dir)/%%_moc.cpp: %%_moc.h
ifeq ($$V,max)
	$(moc) $$< > $$@
else
	@$$(call rule_message,compiling,$$(stylized_target))
	@$(moc) $$< > $$@
endif
endef

lib_rules += $(my_lib_rules)
