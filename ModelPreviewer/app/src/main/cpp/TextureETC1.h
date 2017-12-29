//
// Created by Krusty on 29.12.2017.
//

#ifndef MODELPREVIEWER_TEXTUREETC1_H
#define MODELPREVIEWER_TEXTUREETC1_H

#include <GLES3/gl32.h>
#include <android/asset_manager.h>

class TextureETC1 {
public:
    static GLuint load(AAssetManager *assetManager, const char *fileName);

private:
    static unsigned short swapBytes(unsigned short aData);
};


#endif //MODELPREVIEWER_TEXTUREETC1_H
