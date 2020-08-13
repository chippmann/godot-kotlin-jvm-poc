//
// Created by cedric on 12.08.20.
//

#ifndef TESTINTEROP_JVM_H
#define TESTINTEROP_JVM_H


#include <jni.h>

typedef jint(JNICALL *CreateJavaVM)(JavaVM **, void **, void *);

class Jvm {
private:
    static JavaVM *jvm;
    static void createJVM();
    static void loadJvmDll(CreateJavaVM *createJavaVM);

public:
    static void setup();
    static void destroy();
    static JNIEnv_ *env;
};


#endif //TESTINTEROP_JVM_H
