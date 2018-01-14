//
// Copyright by Fischer Thomas & Radlwimmer Stefan
//

#ifndef MODELPREVIEWER_MODEL_H
#define MODELPREVIEWER_MODEL_H


#include "Vertex.h"

class Model {
public:
    Model(GLsizei vertexCount)
        : VertexCount(vertexCount), VertexData(new ModelVertex[VertexCount])
    { }

    ~Model(){
        delete[](VertexData);
    }

    GLint TextureID = 0;
    const GLsizei VertexCount = 0;
    ModelVertex* const VertexData = nullptr;
};


#endif //MODELPREVIEWER_MODEL_H
