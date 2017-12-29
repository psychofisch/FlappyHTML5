
#ifndef MODELPREVIEWER_MODELLOADER_H
#define MODELPREVIEWER_MODELLOADER_H

#include <GLES3/gl32.h>
#include <vector>
#include <android/asset_manager.h>
#include "Model.h"
#include "include/tiny_obj_loader.h"

class ModelLoader {

public:

    static Model* Load(AAssetManager* assetManager, const char* fileName);
    static Model *LoadModelFromObj(AAssetManager *assetManager, const char *fileName);
    static Model *LoadModelFromBin(AAssetManager *assetManager, const char *fileName);

private:
    static bool LoadObjOnAndroid(AAssetManager *assetManager, const char *fileName, tinyobj::attrib_t *attrib,
                                     std::vector<tinyobj::shape_t> *shapes, std::string *err, bool trianglulate);

};


#endif //MODELPREVIEWER_MODELLOADER_H
