LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := gamesdk_static
LOCAL_MODULE_FILENAME := libgamesdk_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/gamesdk/lib/$(TARGET_ARCH_ABI)/libgamesdk.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := swappy_share
LOCAL_MODULE_FILENAME := libswappy_share
LOCAL_SRC_FILES := $(LOCAL_PATH)/gamesdk/lib/$(TARGET_ARCH_ABI)/libswappy.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2djs

LOCAL_MODULE_FILENAME := libcocos2djs

ifeq ($(USE_ARM_MODE),1)
LOCAL_ARM_MODE := arm
endif

LOCAL_SRC_FILES := hellojavascript/main.cpp \
                   hellojavascript/Orbit.cpp \
				   hellojavascript/Renderer.cpp \
				   hellojavascript/Circle.cpp \
				   hellojavascript/Settings.cpp \
				   hellojavascript/Thread.cpp \
				   ../../Classes/AppDelegate.cpp \
				   ../../Classes/jsb_module_register.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/gamesdk/include

LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += gamesdk_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module, cocos)
