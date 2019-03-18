lib_djnn_deps = core base

lib_srcs := src/physics/physics.cpp src/physics/world.cpp

ifeq ($(physics),BOX2D)
lib_cppflags = -Isrc/physics
lib_srcs += $(shell find src/physics/Box2D -name "*.cpp")
lib_srcs += $(shell find src/physics/box2d-backend -name "*.cpp")
endif

ifeq ($(physics),ODE)
lib_srcs += $(shell find src/physics/ode-backend -name "*.cpp")
lib_cppflags += -Isrc/physics `pkg-config --cflags ode`
lib_ldflags += `pkg-config --libs ode ccd`
endif