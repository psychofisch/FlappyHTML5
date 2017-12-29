

#ifndef MODELPREVIEWER_VERTEX_H
#define MODELPREVIEWER_VERTEX_H

#include <GLES3/gl32.h>
#include "include/glm/vec3.hpp"
#include "include/glm/vec2.hpp"

struct Vertex {
    GLfloat pos[3];
    GLfloat rgba[4];
};


struct ModelVertex {
    glm::vec3 pos;
    glm::vec2 uv;
};

#endif //MODELPREVIEWER_VERTEX_H
