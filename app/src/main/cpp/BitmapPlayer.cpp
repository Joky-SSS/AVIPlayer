// Write C++ code here.
//
// Do not forget to dynamically load the C++ library into your application.
//
// For instance,
//
// In MainActivity.java:
//    static {
//       System.loadLibrary("aviplayer");
//    }
//
// Or, in MainActivity.kt:
//    companion object {
//      init {
//         System.loadLibrary("aviplayer")
//      }
//    }
#include <jni.h>
#include <android/bitmap.h>
extern "C" {
#include "avilib/avilib.h"
}

#include "Common.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_jokyxray_aviplayer_AbstractPlayerActivity_open(JNIEnv *env, jclass clazz,
                                                        jstring file_name) {
    avi_t *avi = 0;
    const char *cFileName = env->GetStringUTFChars(file_name, 0);
    if (0 == cFileName) {
        goto exit;
    }
    avi = AVI_open_input_file(cFileName, 1);
    env->ReleaseStringUTFChars(file_name, cFileName);
    if (avi == 0) {
        ThrowException(env, "java/io/IOException", AVI_strerror());
    }
    exit:
    return (jlong) avi;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_jokyxray_aviplayer_AbstractPlayerActivity_getWidth(JNIEnv *env, jclass clazz, jlong avi) {
    return AVI_video_width((avi_t *) avi);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_jokyxray_aviplayer_AbstractPlayerActivity_getHeight(JNIEnv *env, jclass clazz, jlong avi) {
    return AVI_video_height((avi_t *) avi);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_jokyxray_aviplayer_AbstractPlayerActivity_getFrameRate(JNIEnv *env, jclass clazz,
                                                                jlong avi) {
    return AVI_frame_rate((avi_t *) avi);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_jokyxray_aviplayer_AbstractPlayerActivity_close(JNIEnv *env, jclass clazz, jlong avi) {
    AVI_close((avi_t *) avi);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_jokyxray_aviplayer_BitmapPlayerActivity_render(JNIEnv *env, jclass clazz, jlong avi,
                                                        jobject bitmap) {
    jboolean isFrameRead = JNI_FALSE;
    char *frameBuffer = 0;
    long frameSize = 0;
    int keyFrame = 0;
    if (0 > AndroidBitmap_lockPixels(env, bitmap, (void **) &frameBuffer)) {
        ThrowException(env, "java/io/IOException", "unable to lock pixels");
        goto exit;
    }
    frameSize = AVI_read_frame((avi_t *) avi, frameBuffer, &keyFrame);
    if (0 > AndroidBitmap_unlockPixels(env, bitmap)) {
        ThrowException(env, "java/io/IOException", "unable to unlock pixels");
        goto exit;
    }
    if (frameSize > 0) {
        isFrameRead = JNI_TRUE;
    }
    exit:
    return isFrameRead;
}