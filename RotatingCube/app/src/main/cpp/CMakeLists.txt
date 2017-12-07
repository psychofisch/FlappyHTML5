cmake_minimum_required(VERSION 3.4.1)

add_library(gles3jni SHARED
            gles3jni.cpp
            Renderer.cpp)

# Include libraries needed for gles3jni lib
target_link_libraries(gles3jni
                      GLESv2
                      android
                      EGL
                      log
                      m)
