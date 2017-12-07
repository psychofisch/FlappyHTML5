

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
        {{-1.0f,-1.0f,-1.0f},{ 0.435f,  0.602f,  0.223f}},
        {{-1.0f, 1.0f,-1.0f},{ 0.310f,  0.747f,  0.185f}},
        {{ 1.0f,-1.0f, 1.0f},{ 0.597f,  0.770f,  0.761f}},
        {{-1.0f,-1.0f,-1.0f},{ 0.559f,  0.436f,  0.730f}},
        {{ 1.0f,-1.0f,-1.0f},{ 0.359f,  0.583f,  0.152f}},
        {{ 1.0f, 1.0f,-1.0f},{ 0.483f,  0.596f,  0.789f}},
        {{ 1.0f,-1.0f,-1.0f},{ 0.559f,  0.861f,  0.639f}},
        {{-1.0f,-1.0f,-1.0f},{ 0.195f,  0.548f,  0.859f}},
        {{-1.0f,-1.0f,-1.0f},{ 0.014f,  0.184f,  0.576f}},
        {{-1.0f, 1.0f, 1.0f},{ 0.771f,  0.328f,  0.970f}},
        {{-1.0f, 1.0f,-1.0f},{ 0.406f,  0.615f,  0.116f}},
        {{ 1.0f,-1.0f, 1.0f},{ 0.676f,  0.977f,  0.133f}},
        {{-1.0f,-1.0f, 1.0f},{ 0.971f,  0.572f,  0.833f}},
        {{-1.0f,-1.0f,-1.0f},{ 0.140f,  0.616f,  0.489f}},
        {{-1.0f, 1.0f, 1.0f},{ 0.997f,  0.513f,  0.064f}},
        {{-1.0f,-1.0f, 1.0f},{ 0.945f,  0.719f,  0.592f}},
        {{ 1.0f,-1.0f, 1.0f},{ 0.543f,  0.021f,  0.978f}},
        {{ 1.0f, 1.0f, 1.0f},{ 0.279f,  0.317f,  0.505f}},
        {{ 1.0f,-1.0f,-1.0f},{ 0.167f,  0.620f,  0.077f}},
        {{ 1.0f, 1.0f,-1.0f},{ 0.347f,  0.857f,  0.137f}},
        {{ 1.0f,-1.0f,-1.0f},{ 0.055f,  0.953f,  0.042f}},
        {{ 1.0f, 1.0f, 1.0f},{ 0.714f,  0.505f,  0.345f}},
        {{ 1.0f,-1.0f, 1.0f},{ 0.783f,  0.290f,  0.734f}},
        {{ 1.0f, 1.0f, 1.0f},{ 0.722f,  0.645f,  0.174f}},
        {{ 1.0f, 1.0f,-1.0f},{ 0.302f,  0.455f,  0.848f}},
        {{-1.0f, 1.0f,-1.0f},{ 0.225f,  0.587f,  0.040f}},
        {{ 1.0f, 1.0f, 1.0f},{ 0.517f,  0.713f,  0.338f}},
        {{-1.0f, 1.0f,-1.0f},{ 0.053f,  0.959f,  0.120f}},
        {{-1.0f, 1.0f, 1.0f},{ 0.393f,  0.621f,  0.362f}},
        {{ 1.0f, 1.0f, 1.0f},{ 0.673f,  0.211f,  0.457f}},
        {{-1.0f, 1.0f, 1.0f},{ 0.820f,  0.883f,  0.371f}},
        {{ 1.0f,-1.0f, 1.0f},{ 0.982f,  0.099f,  0.879f}}
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
