//
// Copyright by Fischer Thomas & Radlwimmer Stefan
//

#include "ModelLoader.h"
#include "include/glm/vec3.hpp"
#include "include/glm/vec2.hpp"
#include <iostream>
#include <string>
#include <android/asset_manager.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "include/tiny_obj_loader.h"
#include "Helper.h"

Model *ModelLoader::Load(AAssetManager *assetManager, const char *fileName) {

    const char* isObj = ".obj";
    const char* isBin = ".bin";

    size_t lenstr = strlen(fileName);
    size_t lensuffix = 4;

    if (strncmp(fileName + lenstr - lensuffix, isObj, lensuffix) == 0) {
        return LoadModelFromObj(assetManager, fileName);

    } else if (strncmp(fileName + lenstr - lensuffix, isBin, lensuffix) == 0) {
        return LoadModelFromBin(assetManager, fileName);

    } else {
        return nullptr;
    }
}

Model *ModelLoader::LoadModelFromObj(AAssetManager *assetManager, const char *fileName) {
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    tinyobj::attrib_t attribs;
    bool ret = LoadObjOnAndroid(assetManager, fileName, &attribs, &shapes, &err, false);

    if (!ret)
    {
        ALOGE("Cannot open file [%s]", fileName);
        return nullptr;
    }

    tinyobj::shape_t shape = shapes[0];

    Model* model = new Model;
    model->VertexCount = shape.mesh.indices.size();
    model->VertexData = new ModelVertex[model->VertexCount];

    for (size_t vertex = 0; vertex < model->VertexCount; ++vertex)
    {
        tinyobj::index_t indices = shape.mesh.indices[vertex];
        const int vertexIndex = 3 * indices.vertex_index; // --> times 3 for float to glm::vec3 conversion
        const int texCoordIndex = 2 * indices.texcoord_index; // --> times 2 for float to glm::vec2 conversion

        model->VertexData[vertex] = ModelVertex{glm::vec3(attribs.vertices[vertexIndex + 0],
                                                          attribs.vertices[vertexIndex + 1],
                                                          attribs.vertices[vertexIndex + 2]),
                                                glm::vec2(attribs.texcoords[texCoordIndex + 0],
                                                          attribs.texcoords[texCoordIndex + 1])};
    }

    return model;
}

Model *ModelLoader::LoadModelFromBin(AAssetManager *assetManager, const char *fileName) {
    AAsset* assetFile = AAssetManager_open(assetManager, fileName, AASSET_MODE_BUFFER);
    if (!assetFile) {
        ALOGE("Cannot open file [%s]", fileName);
        return 0;
    }

    Model* model = new Model();

    const char *buffer = static_cast<const char*>(AAsset_getBuffer(assetFile));

    unsigned int chunkType = *reinterpret_cast<const unsigned int*>(buffer);
    buffer+=4;
    unsigned int chunkSize = *reinterpret_cast<const unsigned int*>(buffer);
    buffer+=4;
    unsigned int sectionSize = *reinterpret_cast<const unsigned int*>(buffer);
    buffer+=4;

    model->VertexCount = sectionSize / sizeof(ModelVertex);

    model->VertexData = new ModelVertex[model->VertexCount];
    memcpy(model->VertexData, buffer, sectionSize);

    AAsset_close(assetFile);

    return model;
}

bool ModelLoader::LoadObjOnAndroid(AAssetManager *assetManager, const char *fileName, tinyobj::attrib_t *attrib,
                                   std::vector<tinyobj::shape_t> *shapes, std::string *err, bool trianglulate) {
    attrib->vertices.clear();
    attrib->normals.clear();
    attrib->texcoords.clear();
    shapes->clear();

    std::stringstream errss;

    AAsset* assetFile = AAssetManager_open(assetManager, fileName, AASSET_MODE_BUFFER);
    if (!assetFile) {
        errss << "Cannot open file [" << fileName << "]" << std::endl;
        if (err) {
            (*err) = errss.str();
        }
        return false;
    }

    std::string objText(static_cast<const char*>(AAsset_getBuffer(assetFile)));
    std::stringstream ss(objText);

    bool couldRead = tinyobj::LoadObj(attrib, shapes, nullptr, err, &ss, nullptr, trianglulate);

    AAsset_close(assetFile);
    return couldRead;
}
