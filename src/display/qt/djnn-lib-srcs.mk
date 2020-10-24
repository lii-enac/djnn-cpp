local_dir = $(src_dir)/display/qt

lib_srcs += $(call rwildcard,$(local_dir)/,*.cpp)

lib_objs += $(build_dir)/$(local_dir)/qt_window_moc.o
lib_srcgens += $(build_dir)/$(local_dir)/qt_window_moc.cpp

#define my_lib_rules
$(build_dir)/%_moc.cpp: %_moc.h
ifeq ($V,max)
	$(moc) $< > $@
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(moc) $< > $@
endif
#lib_rules += $(my_lib_rules)
