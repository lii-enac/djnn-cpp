#lib_djnn_deps = core
lib_djnn_deps = gui display base core # _DEBUG_SEE_CREATION_DESTRUCTION_ORDER
lib_srcs += $(call rwildcard,src/utils/,*.cpp)