lib_djnn_deps = core
lib_srcs := src/phidgets/phidgets.cpp

ifeq ($(os),Linux)
	lib_srcs += $(shell find src/phidgets/unix -name "*.cpp")
	lib_ldflags = -lphidget22
else ifeq ($(os),Darwin)
	lib_srcs += $(shell find src/phidgets/unix -name "*.cpp")
	lib_cppflags = -I/Library/Frameworks/Phidget22.framework/Headers
	lib_ldflags = -F /Library/Frameworks -framework Phidget22
else
	lib_srcs += src/phidgets/alt/alt_phidgets.cpp
endif
