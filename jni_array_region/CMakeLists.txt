cmake_minimum_required(VERSION 3.4.1)

add_library(jniregion SHARED
           src/main/jni/jniregion.cpp)

target_link_libraries(jniregion
                     android
                     log)
