//
// Copyright by Fischer Thomas & Radlwimmer Stefan
//

#include "Renderer.h"
#include "Helper.h"

static Renderer* g_renderer = NULL;

static void printGlString(const char* name, GLenum s) {
    const char* v = (const char*)glGetString(s);
    ALOGV("GL %s: %s\n", name, v);
}