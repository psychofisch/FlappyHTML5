//
// Copyright by Fischer Thomas & Radlwimmer Stefan
//

#include "Renderer.h"

#include "include/glm/ext.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

Renderer::Renderer()
        :   mLastFrameNs(0),
            mAngularVelocity(0, 1, 0),
            mEglContext(eglGetCurrentContext()),
            mProgram(0),
            mVB(0),
            mPosAttrib(-1),
            mColorAttrib(-1),
            mModelMatrixUniform(-1),
            mProjMatrixUniform(-1),
            mOffset(0,0,-5),
            mScale(1)
{
    srand (time(NULL));

    float pitch = rand() % 360;
    float yaw = rand() % 360;
    float roll = rand() % 360;

    mRotation = glm::quat(glm::vec3(glm::radians(pitch), glm::radians(yaw), glm::radians(roll)));
    calcModelMatrix();
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

    mPosAttrib = glGetAttribLocation(mProgram, "pos");
    mColorAttrib = glGetAttribLocation(mProgram, "color");
    mModelMatrixUniform = glGetUniformLocation(mProgram, "model");
    mProjMatrixUniform = glGetUniformLocation(mProgram, "projection");

    glGenBuffers(1, &mVB);
    glBindBuffer(GL_ARRAY_BUFFER, mVB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE), &CUBE[0], GL_STATIC_DRAW);

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
    checkGlError("Renderer::render");
}

//Add Rotation and Update ModelMatrix
void Renderer::step() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t nowNs = now.tv_sec*1000000000ull + now.tv_nsec;
    if (mLastFrameNs > 0) {
        float dt = float(nowNs - mLastFrameNs) * 0.000000001f;

        mRotation = glm::normalize(mRotation * glm::quat(mAngularVelocity * dt));
    }

    calcModelMatrix();
    mLastFrameNs = nowNs;
}

//Draw Cube
void Renderer::draw() {
    glUseProgram(mProgram);

    glBindBuffer(GL_ARRAY_BUFFER, mVB);
    glVertexAttribPointer(mPosAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, pos));
    glVertexAttribPointer(mColorAttrib, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, rgba));
    glEnableVertexAttribArray(mPosAttrib);
    glEnableVertexAttribArray(mColorAttrib);

    glUniformMatrix4fv(mModelMatrixUniform, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
    glUniformMatrix4fv(mProjMatrixUniform, 1, GL_FALSE, glm::value_ptr(mProjectMatrix));
    glDrawArrays(GL_TRIANGLES, 0, VERTEX_PER_CUBE);
}

//Check for OpenGl Errors
bool Renderer::checkGlError(const char* funcName) {
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        ALOGE("GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}

//Create Sub-Shader
GLuint Renderer::createShader(GLenum shaderType, const char* src) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        checkGlError("glCreateShader");
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
        checkGlError("glCreateProgram");
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
    const glm::mat4 scale = glm::scale(glm::mat4(1), mScale);
    const glm::mat4 translation = glm::translate(glm::mat4(1), mOffset);
    const glm::mat4 rotation = glm::toMat4(glm::normalize(mRotation));

    mModelMatrix = translation * rotation * scale;
}

//Calculate ProjectionMatrix
void Renderer::calcProjectionMatrix(unsigned int w, unsigned int h) {
    float aspectRatio = (float) w / (float) h;
    mProjectMatrix = glm::perspective(45.0f, aspectRatio, 0.1f, 10.0f);
}
