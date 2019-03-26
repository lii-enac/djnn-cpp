lib_djnn_deps = core


# Ivy dependancy
# svn co https://svn.tls.cena.fr/svn/ivy/ivy-c/trunk ivy-c
# cd ivy-c/src
# MACOS
# make -f Makefile.osx static-libs shared-libs
# sudo make -f Makefile.osx installlibs includes
# linux
# make static-libs shared-libs
# sudo make installlibs includes

ifeq ($(os),Darwin)
lib_cppflags = -I/opt/local/include/
lib_ldflags = -L/opt/local/lib -livy
endif
ifeq ($(os),Linux)
lib_cppflags = -I/usr/local/include/
lib_ldflags = -L/usr/local/lib64 -livy -lpcre
endif
ifeq ($(os),$(osmingw))
lib_cppflags = -I../ivy-c
lib_ldflags = -L../ivy-c/src -livy -lws2_32 -L/mingw64/lib -lpcre
endif

lib_srcs := $(shell find src/comms -name "*.cpp")