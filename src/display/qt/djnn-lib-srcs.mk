local_dir = $(src_dir)/display/qt

$(build_dir)/$(local_dir)/moc_MyQWindow.cpp: $(local_dir)/my_qwindow.h
ifeq ($V,max)
	$(moc) $< > $@
else
	@$(call rule_message,compiling,$(stylized_target))
	@$(moc) $< > $@
endif


lib_objs += $(build_dir)/$(local_dir)/moc_MyQWindow.o
lib_srcgens += $(build_dir)/$(local_dir)/moc_MyQWindow.cpp
lib_srcs += $(call rwildcard,$(local_dir)/,*.cpp)

