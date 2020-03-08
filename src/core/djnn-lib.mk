lib_srcs := src/core/core.cpp
lib_srcs += $(shell find src/core/ontology -name "*.cpp")
lib_srcs += $(shell find src/core/control -name "*.cpp")
lib_srcs += $(shell find src/core/tree -name "*.cpp")
lib_srcs += $(shell find src/core/execution -name "*.cpp")
lib_srcs += $(shell find src/core/serializer -name "*.cpp")
lib_srcs += $(shell find src/core/utils -name "*.cpp")

#lib_srcs += $(shell find src/core/utils/ext/remotery -name "*.c")
#lib_cflags += -DRMT_ENABLED=1 -DRMT_USE_OPENGL=0
CFLAGS += -DRMT_ENABLED=0 # remotery - get rid of any call


ifneq ($(os),FreeRTOS)
lib_srcs += $(shell find src/core/xml -name "*.cpp")
endif
ifneq ($(os),$(filter $(os),FreeRTOS em))
lib_ldflags += -lexpat -lcurl
endif

src/core/djnn-srcs.mk:
	srcs=""
	srcs+=`find src/core -name "*.cpp"`
	echo $$srcs
	echo `find src/core -name "*.cpp"`
	echo lib_srcs := $$srcs > $@

.PHONY: src/core/djnn-srcs.mk
