cmake_minimum_required(VERSION 3.4.1)

add_library(threadmutex SHARED
           src/main/jni/threadmutex.cpp)

target_link_libraries(threadmutex
                     android
                     log)
