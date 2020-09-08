cmake_minimum_required(VERSION 3.4.1)

add_library(threadnoleak SHARED
           src/main/jni/threadnoleak.cpp)

target_link_libraries(threadnoleak
                     android
                     log)
