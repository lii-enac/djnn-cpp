lib_djnn_deps = core


# Ivy dependancy
# svn co https://svn.tls.cena.fr/svn/ivy/ivy-c/trunk ivy-c
# cd ivy-c/src
# make -f Makefile.osx static-libs shared-libs
# sudo make -f Makefile.osx installlibs includes

ifeq ($(os),Darwin)
lib_cppflags = -I/opt/local/include/
lib_ldflags = -L/opt/local/lib -livy
#lib_ldflags = -L/usr/local/lib64 -livy -lpcre
endif
ifeq ($(os),Linux)
#lib_cppflags = -I/opt/local/include/
#lib_ldflags = -L/opt/local/lib -livy
endif

lib_srcs := $(shell find src/comms -name "*.cpp")