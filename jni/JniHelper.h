//
// Created by cedric on 13.08.20.
//

#ifndef TESTINTEROP_JNIHELPER_H
#define TESTINTEROP_JNIHELPER_H


#include <jni.h>

class JniHelper {
private:
    static _jobject* urlClassLoader;
    static jmethodID loadClassMethodId;
public:
    static void setupClassLoader(const char *classPath1);
    static jclass getAnCreateClass(const char* fqname);
    static jclass getClass(const char* fqname);
    static jclass createClass(jclass clazz);
};


#endif //TESTINTEROP_JNIHELPER_H
