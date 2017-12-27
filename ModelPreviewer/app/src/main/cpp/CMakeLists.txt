cmake_minimum_required(VERSION 3.4.1)

add_library(gles2jni SHARED
            gles2jni.cpp
            Renderer.cpp)

# Include libraries needed for gles2jni lib
target_link_libraries(gles2jni
                      GLESv2
                      android
                      EGL
                      log
                      m)
