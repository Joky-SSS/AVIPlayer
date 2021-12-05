//
// Created by Joky on 2021/11/28 0028.
//

#include "Common.h"

void ThrowException(JNIEnv *env,const char *className,const char *message){
    jclass clazz = env->FindClass(className);
    if (0!= clazz){
        env->ThrowNew(clazz,message);
        env->DeleteLocalRef(clazz);
    }
}