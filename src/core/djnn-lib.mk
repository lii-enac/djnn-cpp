lib_ldflags = -lexpat -lcurl -lpthread
lib_srcs := $(shell find src/core -name "*.cpp")
lib_srcs += $(shell find src/core/execution -name "*.cpp")
lib_srcs += $(shell find src/core/xml -name "*.cpp")
