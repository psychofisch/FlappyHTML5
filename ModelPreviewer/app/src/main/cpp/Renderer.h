//
// Copyright by Fischer Thomas & Radlwimmer Stefan
//

#ifndef ROTATINGCUBE_RENDERER_H
#define ROTATINGCUBE_RENDERER_H

#include <android/log.h>

#define DEBUG 1

#define LOG_TAG "GLES3JNI"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif


#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <EGL/egl.h>
#include <GLES3/gl32.h>

#include <math.h>
#include "include/glm/detail/type_mat.hpp"
#include "include/glm/detail/type_vec3.hpp"
#include "include/glm/gtc/quaternion.hpp"

struct Vertex {
    GLfloat pos[3];
    GLfloat rgba[4];
};

static const int VERTEX_PER_CUBE = 36;

const Vertex CUBE[VERTEX_PER_CUBE] = {
        {{-1.0f, -1.0f, -1.0f} , {0.583f, 0.771f, 0.014f}},
        {{-1.0f, -1.0f,  1.0f} , {0.609f, 0.115f, 0.436f}},
        {{-1.0f,  1.0f,  1.0f} , {0.327f, 0.483f, 0.844f}},
        {{ 1.0f,  1.0f, -1.0f} , {0.822f, 0.569f, 0.201f}},
        {{-1.0f, -1.0f, -1.0f} , {0.583f, 0.771f, 0.014f}},
        {{-1.0f,  1.0f, -1.0f} , {0.310f, 0.747f, 0.185f}},
        {{ 1.0f, -1.0f,  1.0f} , {0.597f, 0.770f, 0.761f}},
        {{-1.0f, -1.0f, -1.0f} , {0.583f, 0.771f, 0.014f}},
        {{ 1.0f, -1.0f, -1.0f} , {0.359f, 0.583f, 0.152f}},
        {{ 1.0f,  1.0f, -1.0f} , {0.822f, 0.569f, 0.201f}},
        {{ 1.0f, -1.0f, -1.0f} , {0.359f, 0.583f, 0.152f}},
        {{-1.0f, -1.0f, -1.0f} , {0.583f, 0.771f, 0.014f}},
        {{-1.0f, -1.0f, -1.0f} , {0.583f, 0.771f, 0.014f}},
        {{-1.0f,  1.0f,  1.0f} , {0.327f, 0.483f, 0.844f}},
        {{-1.0f,  1.0f, -1.0f} , {0.310f, 0.747f, 0.185f}},
        {{ 1.0f, -1.0f,  1.0f} , {0.597f, 0.770f, 0.761f}},
        {{-1.0f, -1.0f,  1.0f} , {0.609f, 0.115f, 0.436f}},
        {{-1.0f, -1.0f, -1.0f} , {0.583f, 0.771f, 0.014f}},
        {{-1.0f,  1.0f,  1.0f} , {0.327f, 0.483f, 0.844f}},
        {{-1.0f, -1.0f,  1.0f} , {0.609f, 0.115f, 0.436f}},
        {{ 1.0f, -1.0f,  1.0f} , {0.597f, 0.770f, 0.761f}},
        {{ 1.0f,  1.0f,  1.0f} , {0.279f, 0.317f, 0.505f}},
        {{ 1.0f, -1.0f, -1.0f} , {0.359f, 0.583f, 0.152f}},
        {{ 1.0f,  1.0f, -1.0f} , {0.822f, 0.569f, 0.201f}},
        {{ 1.0f, -1.0f, -1.0f} , {0.359f, 0.583f, 0.152f}},
        {{ 1.0f,  1.0f,  1.0f} , {0.279f, 0.317f, 0.505f}},
        {{ 1.0f, -1.0f,  1.0f} , {0.597f, 0.770f, 0.761f}},
        {{ 1.0f,  1.0f,  1.0f} , {0.279f, 0.317f, 0.505f}},
        {{ 1.0f,  1.0f, -1.0f} , {0.822f, 0.569f, 0.201f}},
        {{-1.0f,  1.0f, -1.0f} , {0.310f, 0.747f, 0.185f}},
        {{ 1.0f,  1.0f,  1.0f} , {0.279f, 0.317f, 0.505f}},
        {{-1.0f,  1.0f, -1.0f} , {0.310f, 0.747f, 0.185f}},
        {{-1.0f,  1.0f,  1.0f} , {0.327f, 0.483f, 0.844f}},
        {{ 1.0f,  1.0f,  1.0f} , {0.279f, 0.317f, 0.505f}},
        {{-1.0f,  1.0f,  1.0f} , {0.327f, 0.483f, 0.844f}},
        {{ 1.0f, -1.0f,  1.0f} , {0.597f, 0.770f, 0.761f}}
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
    ~Renderer();

    // Public Library Interface
    bool init();
    void resize(int w, int h);
    void render();
    void zoom(float zoom);
    void rotate(float rotateX, float rotateY);

private:
    //GameLoop
    void step();
    void draw();

    //OpenGl Helper Methods
    bool checkGlError(const char* funcName);
    GLuint createShader(GLenum shaderType, const char* src);
    GLuint createProgram(const char* vtxSrc, const char* fragSrc);

    //Update Projection and Model Matrix
    void calcModelMatrix();
    void calcProjectionMatrix(unsigned int w, unsigned int h);

    //Rendering Variables
    const EGLContext mEglContext;
    GLuint mProgram;
    GLuint mVB;
    GLint mPosAttrib;
    GLint mColorAttrib;
    GLint mModelMatrixUniform;
    GLint mProjMatrixUniform;
    GLfloat mScaleFactor;
    //GLfloat mRotation;

    //Rotation Variables
    glm::vec3 mAngularVelocity;
    glm::quat mRotation;
    uint64_t mLastFrameNs;

    //Rendering Matrices
    glm::mat4 mProjectMatrix;
    glm::mat4 mModelMatrix;

    //Static Positioning Parameters
    const glm::vec3 mOffset = glm::vec3(0,0,-5);

    const GLfloat mMaxScaleFactor = 5.f;
    const GLfloat mMinScaleFactor = .1f;
    const GLfloat mScaleSpeed = 1.f;

    const GLfloat mRotationSpeed = 0.002f;

};


#endif //ROTATINGCUBE_RENDERER_H
