local_dir := $(src_dir)/gui/gl_

lib_cppflags += -I$(local_dir)
#-I$(local_dir)/khronos
lib_cflags := $(lib_cppflags)

# ifeq ($(os),Darwin)
# # macos opengl
# lib_cflags += -DGL_SILENCE_DEPRECATION=1
# lib_cppflags += -DGL_SILENCE_DEPRECATION=1 -DUSE_GL2D=1 -DUSE_TARP=0
# lib_ldflags += -framework OpenGL
# # SwiftShader
# # lib_cflags += -DUSE_GL_ES_VERSION_2_0
# # lib_cppflags += -DUSE_GL_ES_VERSION_2_0 -DUSE_GL2D=1 -DUSE_TARP=0
# # lib_ldflags += -L/Users/conversy/src-ext/SwiftShader/build -lGLESv2 -lEGL
# endif

# ifeq ($(os),Linux)
# lib_ldflags += -lGL
# #lib_ldflags += -L/opt/vc/lib -lbrcmGLESv2 -lm -lbcm_host
# #warning rpi: glad necessitates to add libbrcmGLESv2 line 89?
# #lib_cflags += -DUSE_GL_ES_VERSION_2_0
# #lib_cppflags += -DUSE_GL_ES_VERSION_2_0
# #lib_cppflags += -I$(LOCALDIR)/include -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux -I$(src_dir)/gui/gl/gl2d/ext/khronos
# endif

# glad-srcs := $(shell find $(local_dir)/glad -name "*.c") 

# ext-srcs := \
# $(glad-srcs)

srcs := $(shell find $(local_dir) -name "*.cpp")

lib_srcs +=  $(srcs)
#$(ext-srcs)