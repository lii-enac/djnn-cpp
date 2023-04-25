lib_djnn_deps = comms files animation audio input gui display base exec_env core # physics 
local_dir = $(src_dir)/c_api
lib_srcs += $(wildcard $(local_dir)/*.cpp)

