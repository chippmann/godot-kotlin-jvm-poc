//
// Created by cedric on 13.08.20.
//

#include <vector>
#include <string>
#include "JniHelper.h"
#include "../jvm/Jvm.h"

_jobject* JniHelper::urlClassLoader = nullptr;
jmethodID JniHelper::loadClassMethodId = nullptr;

void JniHelper::setupClassLoader(const char *classPath1) {
    std::vector<std::string> classPath;
    classPath.emplace_back(classPath1);

    jclass urlClass = Jvm::env->FindClass("java/net/URL");
    jobjectArray urlArray = Jvm::env->NewObjectArray(classPath.size(), urlClass, nullptr);

    size_t i = 0;
    for (const std::string &classPathURL : classPath) {
        jstring urlStr = Jvm::env->NewStringUTF(classPathURL.c_str());
        jclass fileClass = Jvm::env->FindClass("java/io/File");
        jmethodID fileCtor = Jvm::env->GetMethodID(fileClass, "<init>", "(Ljava/lang/String;)V");
        _jobject* file = Jvm::env->NewObject(fileClass, fileCtor, urlStr);
        jmethodID toUriMethod = Jvm::env->GetMethodID(fileClass, "toURI", "()Ljava/net/URI;");
        _jobject* uri = Jvm::env->CallObjectMethod(file, toUriMethod);
        jclass uriClass = Jvm::env->FindClass("java/net/URI");
        jmethodID toUrlMethod = Jvm::env->GetMethodID(uriClass, "toURL", "()Ljava/net/URL;");
        _jobject* url = Jvm::env->CallObjectMethod(uri, toUrlMethod);

        Jvm::env->SetObjectArrayElement(urlArray, i++, url);
    }

    jclass threadClass = Jvm::env->FindClass("java/lang/Thread");
    jmethodID threadGetCurrent = Jvm::env->GetStaticMethodID(threadClass, "currentThread", "()Ljava/lang/Thread;");
    _jobject* thread = Jvm::env->CallStaticObjectMethod(threadClass, threadGetCurrent);
    jmethodID threadGetLoader = Jvm::env->GetMethodID(threadClass, "getContextClassLoader", "()Ljava/lang/ClassLoader;");
    _jobject* contextClassLoader = Jvm::env->CallObjectMethod(thread, threadGetLoader);
    jclass urlClassLoaderClass = Jvm::env->FindClass("java/net/URLClassLoader");
    jmethodID urlClassLoaderCtor = Jvm::env->GetMethodID(
            urlClassLoaderClass,
            "<init>",
            "([Ljava/net/URL;Ljava/lang/ClassLoader;)V"
    );
    urlClassLoader = Jvm::env->NewObject(urlClassLoaderClass, urlClassLoaderCtor, urlArray, contextClassLoader);
    jmethodID threadSetLoader = Jvm::env->GetMethodID(threadClass, "setContextClassLoader", "(Ljava/lang/ClassLoader;)V");
    Jvm::env->CallVoidMethod(thread, threadSetLoader, urlClassLoader);
    loadClassMethodId = Jvm::env->GetMethodID(urlClassLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
}

jclass JniHelper::getClass(const char *fqname) {
    jstring classNameUTF = Jvm::env->NewStringUTF(fqname);
    return (jclass) Jvm::env->CallObjectMethod(urlClassLoader, loadClassMethodId, classNameUTF);
}

jclass JniHelper::createClass(jclass clazz) {
    jmethodID classCtor = Jvm::env->GetMethodID(
            clazz,
            "<init>",
            "()V"
    );
    return (jclass) Jvm::env->NewObject(clazz, classCtor);
}

jclass JniHelper::getAnCreateClass(const char *fqname) {
    return createClass(getClass(fqname));
}
