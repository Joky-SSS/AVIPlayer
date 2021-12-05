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
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <malloc.h>

extern "C" {
#include "avilib/avilib.h"
}

#include "Common.h"

struct Instance {
    char *buffer;
    GLuint texture;

    Instance() : buffer(0), texture(0) {

    }
};

extern "C"
JNIEXPORT jlong JNICALL
Java_com_jokyxray_aviplayer_OpenGLPlayerActivity_init(JNIEnv *env, jclass clazz, jlong avi) {
    Instance *instance = nullptr;
    long frameSize = AVI_frame_size((avi_t *) avi, 0);
    if (frameSize < 0) {
        ThrowException(env, "java/io/IOException", "unable to get the frame size");
        goto exit;
    }
    instance = new Instance();
    if (0 == instance) {
        ThrowException(env, "java/io/IOException", "unable to allocate instance");
        goto exit;
    }
    instance->buffer = (char *) malloc(frameSize);
    if (nullptr == instance->buffer) {
        ThrowException(env, "java/io/IOException", "unable to allocate buffer");
        delete instance;
        instance = nullptr;
    }
    exit:
    return (jlong) instance;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_jokyxray_aviplayer_OpenGLPlayerActivity_initSurface(JNIEnv *env, jclass clazz,
                                                             jlong inst, jlong avi) {
    Instance *instance = (Instance *) inst;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &instance->texture);
    glBindTexture(GL_TEXTURE_2D, instance->texture);
    int frameWidth = AVI_video_width((avi_t *) avi);
    int frameHeight = AVI_video_height((avi_t *) avi);
    GLint rect[] = {0, frameHeight, frameWidth, -frameHeight};
    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, rect);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameWidth, frameHeight, 0, GL_RGB,
                 GL_UNSIGNED_SHORT_5_6_5, 0);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_jokyxray_aviplayer_OpenGLPlayerActivity_render(JNIEnv *env, jclass clazz, jlong inst,
                                                        jlong avi) {
    Instance *instance = (Instance *) inst;
    jboolean isFrameRead = JNI_FALSE;
    int keyFrame = 0;
    long frameSize = AVI_read_frame((avi_t *) avi, instance->buffer, &keyFrame);
    if (frameSize < 0) {
        goto exit;
    }
    isFrameRead = JNI_TRUE;
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, AVI_video_width((avi_t *) avi),
                    AVI_video_height((avi_t *) avi), GL_RGB, GL_UNSIGNED_SHORT_5_6_5,
                    instance->buffer);
    //没有这个函数？
//    glDrawTexiOES(0, 0, 0, AVI_video_width((avi_t *) avi),
//                  AVI_video_height((avi_t *) avi));

    exit:
    return isFrameRead;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_jokyxray_aviplayer_OpenGLPlayerActivity_free(JNIEnv *env, jclass clazz, jlong instance) {

}
