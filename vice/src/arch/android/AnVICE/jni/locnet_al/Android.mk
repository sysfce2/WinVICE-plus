LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := locnet_al

CG_SUBDIRS := .\

# Add more subdirs here, like src/subdir1 src/subdir2

MY_PATH := $(LOCAL_PATH)

CG_SRCDIR := $(LOCAL_PATH)

LOCAL_C_INCLUDES :=	$(LOCAL_PATH)/include \
				$(LOCAL_PATH)/../locnet/include \
				$(LOCAL_PATH)/../sdl/include \
				$(LOCAL_PATH)/../vice/src/core/include \
				$(LOCAL_PATH)
				
LOCAL_PATH := $(MY_PATH)

LOCAL_CPPFLAGS := $(LOCAL_CFLAGS)
LOCAL_CXXFLAGS := $(LOCAL_CFLAGS)

#Change C++ file extension as appropriate
LOCAL_CPP_EXTENSION := .cpp

LOCAL_SRC_FILES := $(foreach F, $(CG_SUBDIRS), $(addprefix $(F)/,$(notdir $(wildcard $(LOCAL_PATH)/$(F)/*.cpp))))

#LOCAL_LDLIBS := -ljnigraphics

LOCAL_ARM_MODE := arm

include $(BUILD_STATIC_LIBRARY)
