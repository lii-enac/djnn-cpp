lib_djnn_deps = core

local_dir := $(src_dir)/extra/phidgets 
lib_srcs := $(local_dir)/phidgets.cpp

lib_headers := $(shell find src/extra/phidgets -name "*.h")

ifeq ($(os),Linux)
	lib_srcs += $(shell find $(local_dir)/unix -name "*.cpp")
	lib_ldflags = -lphidget22
else ifeq ($(os),Darwin)
	lib_srcs += $(shell find $(local_dir)/unix -name "*.cpp")
	lib_cppflags = -I/Library/Frameworks/Phidget22.framework/Headers
	lib_ldflags = -F /Library/Frameworks -framework Phidget22
else
	lib_srcs += $(local_dir)/alt/alt_phidgets.cpp
endif
