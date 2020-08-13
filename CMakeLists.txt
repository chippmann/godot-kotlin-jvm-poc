cmake_minimum_required(VERSION 3.17)
project(testInterop)

set(CMAKE_CXX_STANDARD 14)

find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})
set(JAVA_HOME "/usr/lib/jvm/default")
set(JRE_HOME "${JAVA_HOME}/jre")

add_library(testInterop SHARED library.cpp library.h jvm/Jvm.cpp jvm/Jvm.h Bridge.cpp Bridge.h registration/ClassHandle.h jni/JniHelper.cpp jni/JniHelper.h Godot.cpp Godot.h)

target_link_libraries(testInterop ${CMAKE_DL_LIBS})


target_include_directories(testInterop SYSTEM PUBLIC godot-headers)

file(GLOB JRE_FILES "${JRE_HOME}/*")
file(COPY ${JRE_FILES} DESTINATION jre)

file(COPY java/build/libs/java-0.0.1.jar DESTINATION .)