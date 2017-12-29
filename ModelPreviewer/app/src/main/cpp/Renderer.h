//
// Copyright by Fischer Thomas & Radlwimmer Stefan
//

#ifndef ROTATINGCUBE_RENDERER_H
#define ROTATINGCUBE_RENDERER_H

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

static const char VERTEX_SHADER[] =
        //"#version 100\n"
        "uniform mat4 uModel;\n"
        "uniform mat4 uProjection;\n"
        "attribute vec3 aPos;\n"
        "attribute vec2 aTexCoordinate;\n"
        "varying vec2 vTexCoordinate;\n"
        "void main() {\n"
        "    gl_Position = uProjection * uModel * vec4(aPos, 1.0);\n"
        "    vTexCoordinate = aTexCoordinate;\n"
        "}\n";

static const char FRAGMENT_SHADER[] =
        //"#version 100\n"
        "uniform sampler2D uTexture;\n"
        "precision mediump float;\n"
        "varying vec2 vTexCoordinate;\n"
        "void main() {\n"
        "  gl_FragColor = texture2D(uTexture, vTexCoordinate);\n"
        "}";

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

    //OpenGl Helper Methods
    GLuint createShader(GLenum shaderType, const char* src);
    GLuint createProgram(const char* vtxSrc, const char* fragSrc);

    //Update Projection and Model Matrix
    void calcModelMatrix();
    void calcProjectionMatrix(int w, int h);

    //Rendering Variables
    const EGLContext mEglContext;
    GLuint mProgram;
    GLint mPosAttrib;
    GLint mTexCoordAttrib;
    GLint mModelMatrixUniform;
    GLint mProjMatrixUniform;
    GLint mTextureUniform;
    GLfloat mScaleFactor;

    GLuint mTextureID;
    GLuint mVB;
    GLsizei mVertexCount;

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
