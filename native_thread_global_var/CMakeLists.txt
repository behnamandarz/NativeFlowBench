cmake_minimum_required(VERSION 3.4.1)

add_library(threadglobal SHARED
           src/main/jni/threadglobal.cpp)

target_link_libraries(threadglobal
                     android
                     log)
