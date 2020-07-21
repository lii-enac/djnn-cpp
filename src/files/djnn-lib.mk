lib_djnn_deps = exec_env core base
lib_srcs := src/files/files.cpp src/files/file_reader.cpp src/files/file_writer.cpp src/files/directory_observer.cpp 

ifeq ($(os),Darwin)
	lib_srcs += $(shell find src/files/darwin -name "*.cpp")
else
	lib_srcs += src/files/alt/alt_directory_observer.cpp
endif
