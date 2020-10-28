lib_djnn_deps = base core

lib_srcs += src/physics/physics.cpp src/physics/world.cpp
lib_srcs += $(wildcard src/physics/gen/*.cpp)

ifeq ($(physics),BOX2D)
lib_cppflags = -Isrc/physics
lib_srcs += $(call rwildcard,src/physics/Box2D/,*.cpp)
lib_srcs += $(call rwildcard,src/physics/box2d-backend/,*.cpp)
endif

ifeq ($(physics),ODE)
lib_srcs += $(call rwildcard,src/physics/ode-backend/,*.cpp)
lib_cppflags += -Isrc/physics
lib_pkg += ode ccd
endif