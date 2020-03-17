ifeq ($(crazyflie), CRAZYFLIE)
include src/extra/crazyflie/djnn-lib.mk
endif

ifeq ($(phidgets), PHIDGETS)
#djnn_libs += phidgets
include src/extra/phidgets/djnn-lib.mk
endif
