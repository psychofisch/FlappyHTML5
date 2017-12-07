/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "gles3jni.h"


extern "C" {
    JNIEXPORT void JNICALL Java_krustyfishgl_rotatingcube_GLES3JNILib_init(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_krustyfishgl_rotatingcube_GLES3JNILib_resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_krustyfishgl_rotatingcube_GLES3JNILib_step(JNIEnv* env, jobject obj);
};

#if !defined(DYNAMIC_ES3)
static GLboolean gl3stubInit() {
    return GL_TRUE;
}
#endif

JNIEXPORT void JNICALL
Java_krustyfishgl_rotatingcube_GLES3JNILib_init(JNIEnv* env, jobject obj) {
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
Java_krustyfishgl_rotatingcube_GLES3JNILib_resize(JNIEnv* env, jobject obj, jint width, jint height) {
    if (g_renderer) {
        g_renderer->resize(width, height);
    }
}

JNIEXPORT void JNICALL
Java_krustyfishgl_rotatingcube_GLES3JNILib_step(JNIEnv* env, jobject obj) {
    if (g_renderer) {
        g_renderer->render();
    }
}
