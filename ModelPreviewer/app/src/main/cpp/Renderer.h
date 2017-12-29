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

#include <math.h>
#include "include/glm/detail/type_mat.hpp"
#include "include/glm/detail/type_vec3.hpp"
#include "include/glm/gtc/quaternion.hpp"
#include "ModelLoader.h"
#include "Vertex.h"

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


static const int VERTEX_PER_CUBE = 36;

const ModelVertex CUBE[VERTEX_PER_CUBE] = {
        {{-1.0f, -1.0f, -1.0f} , {0.583f, 0.771f}},
        {{-1.0f, -1.0f,  1.0f} , {0.609f, 0.115f}},
        {{-1.0f,  1.0f,  1.0f} , {0.327f, 0.483f}},
        {{ 1.0f,  1.0f, -1.0f} , {0.822f, 0.569f}},
        {{-1.0f, -1.0f, -1.0f} , {0.583f, 0.771f}},
        {{-1.0f,  1.0f, -1.0f} , {0.310f, 0.747f}},
        {{ 1.0f, -1.0f,  1.0f} , {0.597f, 0.770f}},
        {{-1.0f, -1.0f, -1.0f} , {0.583f, 0.771f}},
        {{ 1.0f, -1.0f, -1.0f} , {0.359f, 0.583f}},
        {{ 1.0f,  1.0f, -1.0f} , {0.822f, 0.569f}},
        {{ 1.0f, -1.0f, -1.0f} , {0.359f, 0.583f}},
        {{-1.0f, -1.0f, -1.0f} , {0.583f, 0.771f}},
        {{-1.0f, -1.0f, -1.0f} , {0.583f, 0.771f}},
        {{-1.0f,  1.0f,  1.0f} , {0.327f, 0.483f}},
        {{-1.0f,  1.0f, -1.0f} , {0.310f, 0.747f}},
        {{ 1.0f, -1.0f,  1.0f} , {0.597f, 0.770f}},
        {{-1.0f, -1.0f,  1.0f} , {0.609f, 0.115f}},
        {{-1.0f, -1.0f, -1.0f} , {0.583f, 0.771f}},
        {{-1.0f,  1.0f,  1.0f} , {0.327f, 0.483f}},
        {{-1.0f, -1.0f,  1.0f} , {0.609f, 0.115f}},
        {{ 1.0f, -1.0f,  1.0f} , {0.597f, 0.770f}},
        {{ 1.0f,  1.0f,  1.0f} , {0.279f, 0.317f}},
        {{ 1.0f, -1.0f, -1.0f} , {0.359f, 0.583f}},
        {{ 1.0f,  1.0f, -1.0f} , {0.822f, 0.569f}},
        {{ 1.0f, -1.0f, -1.0f} , {0.359f, 0.583f}},
        {{ 1.0f,  1.0f,  1.0f} , {0.279f, 0.317f}},
        {{ 1.0f, -1.0f,  1.0f} , {0.597f, 0.770f}},
        {{ 1.0f,  1.0f,  1.0f} , {0.279f, 0.317f}},
        {{ 1.0f,  1.0f, -1.0f} , {0.822f, 0.569f}},
        {{-1.0f,  1.0f, -1.0f} , {0.310f, 0.747f}},
        {{ 1.0f,  1.0f,  1.0f} , {0.279f, 0.317f}},
        {{-1.0f,  1.0f, -1.0f} , {0.310f, 0.747f}},
        {{-1.0f,  1.0f,  1.0f} , {0.327f, 0.483f}},
        {{ 1.0f,  1.0f,  1.0f} , {0.279f, 0.317f}},
        {{-1.0f,  1.0f,  1.0f} , {0.327f, 0.483f}},
        {{ 1.0f, -1.0f,  1.0f} , {0.597f, 0.770f}}
};

static const char VERTEX_SHADER[] =
        "#version 100\n"
        "uniform mat4 model;\n"
        "uniform mat4 projection;\n"
        "attribute vec3 pos;\n"
        "attribute vec2 uv;\n"
        "varying vec2 vTexCoord;\n"
        "void main() {\n"
        "    vTexCoord = uv;\n"
        "    gl_Position = projection * model * vec4(pos, 1.0);\n"
        "}\n";

static const char FRAGMENT_SHADER[] =
        "#version 100\n"
        "precision mediump float;\n"
        "varying vec2 vTexCoord;\n"
        "uniform sampler2D ourTexture;\n"
        "void main() {\n"
        "//    gl_FragColor = texture(ourTexture, vTexCoord);\n"
        "    gl_FragColor = vec4(0.8, 0.8, 0.8, 1);\n"
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
    void setModel(Model *model);

private:
    //GameLoop
    void step();
    void draw();

    //Set Model from UI --> no OpenGl Context available
    void LoadModel();

    //OpenGl Helper Methods
    GLuint createShader(GLenum shaderType, const char* src);
    GLuint createProgram(const char* vtxSrc, const char* fragSrc);

    //Update Projection and Model Matrix
    void calcModelMatrix();
    void calcProjectionMatrix(int w, int h);

    //Rendering Variables
    const EGLContext mEglContext;
    GLuint mProgram;
    GLuint mVB;
    GLsizei mVertexCount;
    GLint mPosAttrib;
    GLint mTexCoordAttrib;
    GLint mModelMatrixUniform;
    GLint mProjMatrixUniform;
    GLfloat mScaleFactor;

    Model* mNextModel = nullptr;

    //Rotation Variables
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
