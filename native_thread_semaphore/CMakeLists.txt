cmake_minimum_required(VERSION 3.4.1)

add_library(threadglobal SHARED
           src/main/jni/threadsemaphore.cpp)

target_link_libraries(threadsemaphore
                     android
                     log)
