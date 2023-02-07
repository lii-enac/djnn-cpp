lib_djnn_deps = core exec_env

local_dir := $(src_dir)/extra/phidgets 
lib_srcs += $(src_dir)/extra/phidgets/phidgets.cpp

ifeq ($(os),Linux)
	lib_srcs += $(call rwildcard,$(src_dir)/extra/phidgets/unix/,*.cpp)
	lib_ldflags += -lphidget22
else ifeq ($(os),Darwin)
	lib_srcs += $(call rwildcard,$(src_dir)/extra/phidgets/unix/,*.cpp)
	lib_cppflags += -I/Library/Frameworks/Phidget22.framework/Headers
	lib_ldflags += -F /Library/Frameworks -framework Phidget22
else
	#lib_srcs += $(local_dir)/alt/alt_phidgets.cpp
endif