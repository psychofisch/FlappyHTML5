cmake_minimum_required(VERSION 3.4.1)

add_definitions("-DDYNAMIC_ES3")

add_library(gles3jni SHARED
            gl3stub_wrapper.c
            gles3jni.cpp
            Renderer.cpp)

# Include libraries needed for gles3jni lib
target_link_libraries(gles3jni
                      GLESv2
                      android
                      EGL
                      log
                      m)
