cmake_minimum_required(VERSION 3.4.1)

add_library(libccopy SHARED
           src/main/jni/libccopy.cpp)

target_link_libraries(libccopy
                     android
                     log)
