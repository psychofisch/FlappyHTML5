//
// Copyright by Fischer Thomas & Radlwimmer Stefan
//

#ifndef MODELPREVIEWER_MODEL_H
#define MODELPREVIEWER_MODEL_H


#include "Vertex.h"

class Model {
public:
    GLint TextureID = 0;
    GLsizei VertexCount = 0;
    ModelVertex* VertexData = nullptr;
};


#endif //MODELPREVIEWER_MODEL_H
