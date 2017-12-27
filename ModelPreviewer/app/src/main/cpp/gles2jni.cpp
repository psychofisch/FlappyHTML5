//
// Copyright by Fischer Thomas & Radlwimmer Stefan
//

#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "gles2jni.h"


extern "C" {
    JNIEXPORT void JNICALL Java_krustyfishgl_modelpreviewer_GLES2JNILib_init(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_krustyfishgl_modelpreviewer_GLES2JNILib_resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_krustyfishgl_modelpreviewer_GLES2JNILib_step(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_krustyfishgl_modelpreviewer_GLES2JNILib_zoom(JNIEnv* env, jobject obj, jfloat zoom);
    JNIEXPORT void JNICALL Java_krustyfishgl_modelpreviewer_GLES2JNILib_rotate(JNIEnv* env, jobject obj, jfloat rotateX, jfloat rotateY);
};

JNIEXPORT void JNICALL
Java_krustyfishgl_modelpreviewer_GLES2JNILib_init(JNIEnv* env, jobject obj) {
    if (g_renderer) {
        delete g_renderer;
        g_renderer = NULL;
    }

    printGlString("Version", GL_VERSION);
    printGlString("Vendor", GL_VENDOR);
    printGlString("Renderer", GL_RENDERER);
    printGlString("Extensions", GL_EXTENSIONS);

   g_renderer = new Renderer();
    if (!g_renderer->init()) {
        delete g_renderer;
        g_renderer = NULL;
    }
}

JNIEXPORT void JNICALL
Java_krustyfishgl_modelpreviewer_GLES2JNILib_resize(JNIEnv* env, jobject obj, jint width, jint height) {
    if (g_renderer) {
        g_renderer->resize(width, height);
    }
}

JNIEXPORT void JNICALL
Java_krustyfishgl_modelpreviewer_GLES2JNILib_step(JNIEnv* env, jobject obj) {
    if (g_renderer) {
        g_renderer->render();
    }
}

JNIEXPORT void JNICALL
Java_krustyfishgl_modelpreviewer_GLES2JNILib_zoom(JNIEnv* env, jobject obj, jfloat zoom){
    if (g_renderer) {
        g_renderer->zoom(zoom);
    }
}

JNIEXPORT void JNICALL
Java_krustyfishgl_modelpreviewer_GLES2JNILib_rotate(JNIEnv* env, jobject obj, jfloat rotateX, jfloat rotateY){
    if (g_renderer) {
        g_renderer->rotate(rotateX, rotateY);
    }
}
