ROBOTISOP2_FRAMEWORK_PATH = ./robotis/Framework

CXX_SOURCES = \
  $(ROBOTISOP2_FRAMEWORK_PATH)/src/math/Matrix.cpp \
  $(ROBOTISOP2_FRAMEWORK_PATH)/src/math/Vector.cpp \
  $(ROBOTISOP2_FRAMEWORK_PATH)/src/math/Point.cpp \
  $(ROBOTISOP2_FRAMEWORK_PATH)/src/MX28.cpp \
  $(ROBOTISOP2_FRAMEWORK_PATH)/src/motion/JointData.cpp \
  $(ROBOTISOP2_FRAMEWORK_PATH)/src/motion/MotionStatus.cpp \
  $(ROBOTISOP2_FRAMEWORK_PATH)/src/motion/Kinematics.cpp \
  $(ROBOTISOP2_FRAMEWORK_PATH)/src/motion/modules/Action.cpp \
  $(ROBOTISOP2_FRAMEWORK_PATH)/src/motion/modules/Walking.cpp \
  $(ROBOTISOP2_FRAMEWORK_PATH)/src/vision/ImgProcess.cpp \
  $(ROBOTISOP2_FRAMEWORK_PATH)/src/vision/ColorFinder.cpp \
  $(ROBOTISOP2_FRAMEWORK_PATH)/src/vision/Image.cpp
C_SOURCES = \
  $(ROBOTISOP2_FRAMEWORK_PATH)/src/minIni/minIni.c

INCLUDE = -I"$(ROBOTISOP2_FRAMEWORK_PATH)/include"
CFLAGS = -DWEBOTS
USE_C_API = True

space :=
space +=
WEBOTS_HOME_PATH=$(subst $(space),\ ,$(strip $(subst \,/,$(WEBOTS_HOME))))
include $(WEBOTS_HOME_PATH)/resources/Makefile.include
