
#ifndef MODELPREVIEWER_HELPER_H
#define MODELPREVIEWER_HELPER_H

#include <android/log.h>

#define DEBUG 1
#define LOG_TAG "GLES3JNI"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

//========================================================

#include <GLES3/gl32.h>
#include <string>

#define CHECK_GL_ERROR
#ifdef CHECK_GL_ERROR
#define glCheckError() glCheckError_(__FUNCTION__, __LINE__)
#else
#define glCheckError()
#endif

GLenum static glCheckError_(const char *function, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        ALOGE("%s | %s (%i)\n", error.c_str(), function, line);
    }
    return errorCode;
}

#endif //MODELPREVIEWER_HELPER_H
