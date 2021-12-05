#include <jni.h>

extern "C" {
#include "avilib/avilib.h"
}

#include <android/native_window_jni.h>
#include <android/native_window.h>
#include "Common.h"
//
// Created by Joky on 2021/12/5 0005.
//


extern "C"
JNIEXPORT void JNICALL
Java_com_jokyxray_aviplayer_NativeWindowPlayerActivity_init(JNIEnv *env, jclass clazz, jlong avi,
                                                            jobject surface) {
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (nativeWindow == nullptr) {
        ThrowException(env, "java/io/RuntimeException", "Unable to get native window from surface");
        goto exit;
    }
    if (ANativeWindow_setBuffersGeometry(nativeWindow, AVI_video_width((avi_t *) avi),
                                         AVI_video_height((avi_t *) avi), WINDOW_FORMAT_RGB_565)
        < 0) {
        ThrowException(env, "java/io/RuntimeException", "Unable to set buffers geometry");
    }
    ANativeWindow_release(nativeWindow);
    nativeWindow = nullptr;
    exit:
    return;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_jokyxray_aviplayer_NativeWindowPlayerActivity_render(JNIEnv *env, jclass clazz, jlong avi,
                                                              jobject surface) {
    jboolean isFrameRead = JNI_FALSE;
    long frameSize = 0;
    int keyFrame = 0;
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env,surface);
    if (nativeWindow == nullptr){
        ThrowException(env, "java/io/RuntimeException", "Unable to get native window from surface");
        goto exit;
    }
    ANativeWindow_Buffer windowBuffer;
    if (ANativeWindow_lock(nativeWindow,&windowBuffer,0) < 0){
        ThrowException(env, "java/io/RuntimeException", "Unable to lock native window");
        goto release;
    }
    frameSize = AVI_read_frame((avi_t*)avi,(char *)windowBuffer.bits,&keyFrame);
    if (frameSize > 0){
        isFrameRead = true;
    }
    if (ANativeWindow_unlockAndPost(nativeWindow) < 0){
        ThrowException(env, "java/io/RuntimeException", "Unable to unlock and post native window");
        goto release;
    }
    release:
    ANativeWindow_release(nativeWindow);
    nativeWindow = nullptr;
    exit:
    return isFrameRead;
}