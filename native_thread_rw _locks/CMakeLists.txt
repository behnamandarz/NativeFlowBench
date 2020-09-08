cmake_minimum_required(VERSION 3.4.1)

add_library(threadrw SHARED
           src/main/jni/threadrw.cpp)

target_link_libraries(threadrw
                     android
                     log)
