//
// Copyright by Fischer Thomas & Radlwimmer Stefan
//

#include "Renderer.h"

#include "include/glm/ext.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "TextureETC1.h"
#include "Helper.h"

Renderer::Renderer()
        :   mLastFrameNs(0),
            mEglContext(eglGetCurrentContext()),
            mProgram(0),
            mVB(0),
            mVertexCount(0),
            mPosAttrib(-1),
            mTexCoordAttrib(-1),
            mModelMatrixUniform(-1),
            mProjMatrixUniform(-1),
            mScaleFactor(1),
            mTextureID(0)
{
}

Renderer::~Renderer() {
    if (eglGetCurrentContext() != mEglContext)
        return;
    glDeleteBuffers(1, &mVB);
    glDeleteProgram(mProgram);
}

///Setup Vertex Buffer and OpenGl Context
bool Renderer::init() {
    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (!mProgram)
        return false;

    mPosAttrib = glGetAttribLocation(mProgram, "aPos");
    mTexCoordAttrib = glGetAttribLocation(mProgram, "aTexCoordinate");
    mModelMatrixUniform = glGetUniformLocation(mProgram, "uModel");
    mProjMatrixUniform = glGetUniformLocation(mProgram, "uProjection");
    mTextureUniform = glGetUniformLocation(mProgram, "uTexture");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    ALOGV("Using OpenGL ES 2.0 renderer");
    return true;
}

//Reset Viewport and ProjectionMatrix
void Renderer::resize(int w, int h) {
    calcProjectionMatrix(w, h);

    mLastFrameNs = 0;

    glViewport(0, 0, w, h);
}

//Update, Flush and Render
void Renderer::render() {
    step();

    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
    glCheckError();
}

//Add Rotation and Update ModelMatrix
void Renderer::step() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t nowNs = now.tv_sec*1000000000ull + now.tv_nsec;
    if (mLastFrameNs > 0) {
        float dt = float(nowNs - mLastFrameNs) * 0.000000001f;

        //update here?
    }

    mLastFrameNs = nowNs;
}

//Draw Cube
void Renderer::draw() {
    if (mVertexCount == 0)
        return;

    glUseProgram(mProgram);
    glCheckError();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glUniform1i(mTextureUniform, 1);

    glBindBuffer(GL_ARRAY_BUFFER, mVB);
    glUniformMatrix4fv(mModelMatrixUniform, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
    glUniformMatrix4fv(mProjMatrixUniform, 1, GL_FALSE, glm::value_ptr(mProjectMatrix));
    glCheckError();
    glDrawArrays(GL_TRIANGLES, 0, mVertexCount);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError();
}

//Create Sub-Shader
GLuint Renderer::createShader(GLenum shaderType, const char* src) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        glCheckError();
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                ALOGE("Could not compile %s shader:\n%s\n",
                      shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                      infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

//Create Shader-Program
GLuint Renderer::createProgram(const char* vtxSrc, const char* fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader)
        goto exit;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader)
        goto exit;

    program = glCreateProgram();
    if (!program) {
        glCheckError();
        goto exit;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        ALOGE("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                ALOGE("Could not link program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

    exit:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}

//Calculate ModelMatrix
void Renderer::calcModelMatrix() {
    const glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(mScaleFactor));
    const glm::mat4 translation = glm::translate(glm::mat4(1), mOffset);
    const glm::mat4 rotation = glm::toMat4(glm::normalize(mRotation));

    mModelMatrix = translation * rotation * scale;
}

//Calculate ProjectionMatrix
void Renderer::calcProjectionMatrix(int w, int h) {
    float aspectRatio = (float) w / (float) h;
    mProjectMatrix = glm::perspective(45.0f, aspectRatio, 0.1f, 10.0f);
}

//Add new zoom factor
void Renderer::zoom(float zoom) {
    // Don't let the object get too small or too large.
    mScaleFactor = std::max(mMinScaleFactor, std::min(mScaleFactor * std::pow(zoom, mScaleSpeed), mMaxScaleFactor));

    calcModelMatrix();
}

//Add new rotation
void Renderer::rotate(float rotateX, float rotateY) {
    mRotation = glm::normalize(glm::quat(glm::vec3(-rotateY, -rotateX, 1) * mRotationSpeed) * mRotation);

    calcModelMatrix();
}

//Copy Model data to GPU
void Renderer::setModel(Model *model) {
    //Clear old data
    glDeleteBuffers(1, &mVB);
    glDeleteTextures(1, &mTextureID);

    //Allocate new data
    glGenBuffers(1, &mVB);
    mTextureID = model->TextureID;
    mVertexCount = model->VertexCount;
    glCheckError();

    //Setup vbo
    glBindBuffer(GL_ARRAY_BUFFER, mVB);
    glBufferData(GL_ARRAY_BUFFER, mVertexCount * sizeof(ModelVertex), model->VertexData, GL_STATIC_DRAW);
    glCheckError();
    glVertexAttribPointer(mPosAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (const GLvoid*)offsetof(ModelVertex, pos));
    glCheckError();
    glVertexAttribPointer(mTexCoordAttrib, 2, GL_FLOAT, GL_TRUE, sizeof(ModelVertex), (const GLvoid*)offsetof(ModelVertex, uv));
    glCheckError();
    glEnableVertexAttribArray(mPosAttrib);
    glEnableVertexAttribArray(mTexCoordAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError();

    //Delete data struct
    delete(model);

    //reset model matrix
    mScaleFactor = 1;
    mRotation = glm::quat();
    calcModelMatrix();

    glCheckError();
}
