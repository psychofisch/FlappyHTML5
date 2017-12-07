

#ifndef ROTATINGCUBE_RENDERER_H
#define ROTATINGCUBE_RENDERER_H

#include <android/log.h>
#include <math.h>

#if DYNAMIC_ES3
#include "gl3stub.h"
#else
// Include the latest possible header file( GL version header )
#if __ANDROID_API__ >= 24
#include <GLES3/gl32.h>
#elif __ANDROID_API__ >= 21
#include <GLES3/gl31.h>
#else
#include <GLES3/gl3.h>
#endif
#endif

#define DEBUG 1

#define LOG_TAG "GLES3JNI"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

#define TWO_PI (2.0 * M_PI)
#define VERTEX_PER_CUBE 36

#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <EGL/egl.h>

#include <math.h>
#include "include/glm/detail/type_mat.hpp"
#include "include/glm/detail/type_vec3.hpp"
#include "include/glm/gtc/quaternion.hpp"

struct Vertex {
    GLfloat pos[3];
    GLfloat rgba[4];
};

const Vertex CUBE[VERTEX_PER_CUBE] = {
        {{-1.0f,-1.0f,-1.0f},{ 0.583f,  0.771f,  0.014f}},
        {{-1.0f,-1.0f, 1.0f},{ 0.609f,  0.115f,  0.436f}},
        {{-1.0f, 1.0f, 1.0f},{ 0.327f,  0.483f,  0.844f}},
        {{ 1.0f, 1.0f,-1.0f},{ 0.822f,  0.569f,  0.201f}},
        {{-1.0f,-1.0f,-1.0f},{ 0.583f,  0.771f,  0.014f}},
        {{-1.0f, 1.0f,-1.0f},{ 0.310f,  0.747f,  0.185f}},
        {{ 1.0f,-1.0f, 1.0f},{ 0.597f,  0.770f,  0.761f}},
        {{-1.0f,-1.0f,-1.0f},{ 0.583f,  0.771f,  0.014f}},
        {{ 1.0f,-1.0f,-1.0f},{ 0.359f,  0.583f,  0.152f}},
        {{ 1.0f, 1.0f,-1.0f},{ 0.822f,  0.569f,  0.201f}},
        {{ 1.0f,-1.0f,-1.0f},{ 0.359f,  0.583f,  0.152f}},
        {{-1.0f,-1.0f,-1.0f},{ 0.583f,  0.771f,  0.014f}},
        {{-1.0f,-1.0f,-1.0f},{ 0.583f,  0.771f,  0.014f}},
        {{-1.0f, 1.0f, 1.0f},{ 0.327f,  0.483f,  0.844f}},
        {{-1.0f, 1.0f,-1.0f},{ 0.310f,  0.747f,  0.185f}},
        {{ 1.0f,-1.0f, 1.0f},{ 0.597f,  0.770f,  0.761f}},
        {{-1.0f,-1.0f, 1.0f},{ 0.609f,  0.115f,  0.436f}},
        {{-1.0f,-1.0f,-1.0f},{ 0.583f,  0.771f,  0.014f}},
        {{-1.0f, 1.0f, 1.0f},{ 0.327f,  0.483f,  0.844f}},
        {{-1.0f,-1.0f, 1.0f},{ 0.609f,  0.115f,  0.436f}},
        {{ 1.0f,-1.0f, 1.0f},{ 0.597f,  0.770f,  0.761f}},
        {{ 1.0f, 1.0f, 1.0f},{ 0.279f,  0.317f,  0.505f}},
        {{ 1.0f,-1.0f,-1.0f},{ 0.359f,  0.583f,  0.152f}},
        {{ 1.0f, 1.0f,-1.0f},{ 0.822f,  0.569f,  0.201f}},
        {{ 1.0f,-1.0f,-1.0f},{ 0.359f,  0.583f,  0.152f}},
        {{ 1.0f, 1.0f, 1.0f},{ 0.279f,  0.317f,  0.505f}},
        {{ 1.0f,-1.0f, 1.0f},{ 0.597f,  0.770f,  0.761f}},
        {{ 1.0f, 1.0f, 1.0f},{ 0.279f,  0.317f,  0.505f}},
        {{ 1.0f, 1.0f,-1.0f},{ 0.822f,  0.569f,  0.201f}},
        {{-1.0f, 1.0f,-1.0f},{ 0.310f,  0.747f,  0.185f}},
        {{ 1.0f, 1.0f, 1.0f},{ 0.279f,  0.317f,  0.505f}},
        {{-1.0f, 1.0f,-1.0f},{ 0.310f,  0.747f,  0.185f}},
        {{-1.0f, 1.0f, 1.0f},{ 0.327f,  0.483f,  0.844f}},
        {{ 1.0f, 1.0f, 1.0f},{ 0.279f,  0.317f,  0.505f}},
        {{-1.0f, 1.0f, 1.0f},{ 0.327f,  0.483f,  0.844f}},
        {{ 1.0f,-1.0f, 1.0f},{ 0.597f,  0.770f,  0.761f}}
};

static const char VERTEX_SHADER[] =
        "#version 100\n"
        "uniform mat4 model;\n"
        "uniform mat4 projection;\n"
        "attribute vec3 pos;\n"
        "attribute vec4 color;\n"
        "varying vec4 vColor;\n"
        "void main() {\n"
        "    gl_Position = projection * model * vec4(pos, 1.0);\n"
        "    vColor = color;\n"
        "}\n";

static const char FRAGMENT_SHADER[] =
        "#version 100\n"
        "precision mediump float;\n"
        "varying vec4 vColor;\n"
        "void main() {\n"
        "    gl_FragColor = vColor;\n"
        "}\n";

class Renderer {
public:
    Renderer();
    virtual ~Renderer();
    bool init();
    void resize(int w, int h);
    void render();

private:

    bool checkGlError(const char* funcName);
    GLuint createShader(GLenum shaderType, const char* src);
    GLuint createProgram(const char* vtxSrc, const char* fragSrc);
    void draw();

    void calcModelMatrix();
    void calcSceneParams(unsigned int w, unsigned int h);
    void step();

    const EGLContext mEglContext;
    GLuint mProgram;
    GLuint mVB;
    GLint mPosAttrib;
    GLint mColorAttrib;
    GLint mModelMatrixUniform;
    GLint mProjMatrixUniform;

    glm::vec3 mAngularVelocity;
    glm::quat mRotation;

    uint64_t mLastFrameNs;
    glm::mat4 mProjectMatrix;
    glm::mat4 mModelMatrix;

    const glm::vec3 mOffset;
    const glm::vec3 mScale;
};


#endif //ROTATINGCUBE_RENDERER_H
