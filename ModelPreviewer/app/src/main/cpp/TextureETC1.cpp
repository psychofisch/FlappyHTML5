//
// Copyright by Fischer Thomas & Radlwimmer Stefan
//

#include <istream>
#include <android/log.h>
#include <sstream>
#include <cstring>
#include <GLES3/gl32.h>
#include "TextureETC1.h"
#include "Helper.h"

unsigned short TextureETC1::swapBytes(unsigned short aData) {
    return ((aData & 0x00FF) << 8) | ((aData & 0xFF00) >> 8);
}

GLuint TextureETC1::load(AAssetManager *assetManager, const char *fileName) {
    AAsset* assetFile = AAssetManager_open(assetManager, fileName, AASSET_MODE_BUFFER);
    if (!assetFile) {
        ALOGE("Cannot open file [%s]", fileName);
        return 0;
    }

    const char *buffer = static_cast<const char*>(AAsset_getBuffer(assetFile));
    const char *header = "PKM ";

    // check if data is ETC1 PKM file - should start with text "PKM " (notice the space in the end)
    // read byte by byte to prevent endianness problems
    if (strncmp(buffer, header, 4) != 0)
    {
        AAsset_close(assetFile);

        ALOGE("File [%s] is no valid PKM formated", fileName);
        return 0;
    }

    buffer+=8;

    // sizes of texture follows: 4 shorts in big endian order
    unsigned short extWidth =  swapBytes(static_cast<unsigned short>(*(buffer+0)));
    unsigned short extHeight = swapBytes(static_cast<unsigned short>(*(buffer+2)));
    unsigned short width =     swapBytes(static_cast<unsigned short>(*(buffer+4)));
    unsigned short height =    swapBytes(static_cast<unsigned short>(*(buffer+6)));

    buffer+=8;

    // calculate size of data with formula (extWidth / 4) * (extHeight / 4) * 8
    unsigned int dataLength = ((extWidth >> 2) * (extHeight >> 2)) << 3;

    GLuint mTextureID;

    // openGL part
    // create and bind texture - all next texture ops will be related to it
    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    // load compressed data (skip 16 bytes of header)
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, 0x8D64, extWidth, extHeight, 0, dataLength, buffer);

    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    glCheckError();

    AAsset_close(assetFile);

    return mTextureID;
}
