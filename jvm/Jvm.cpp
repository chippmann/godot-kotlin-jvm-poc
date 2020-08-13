//
// Created by cedric on 12.08.20.
//

#include <iostream>
#include "Jvm.h"

#ifdef __linux__
#include <dlfcn.h>
#elif _WIN32
#include <Windows.h>
#else

#endif

JavaVM *Jvm::jvm = nullptr;
JNIEnv_ *Jvm::env = nullptr;

void Jvm::setup() {
    createJVM();
}

void Jvm::createJVM() {
    std::cout << "Loading JVM lib" << std::endl;
    CreateJavaVM createJavaVM;
    loadJvmDll(&createJavaVM);
    std::cout << "Creating JVM Instance" << std::endl;
    // https://docs.oracle.com/javase/10/docs/specs/jni/invocation.html
    JavaVMInitArgs vm_args;
    auto *options = new JavaVMOption[3];
    options[0].optionString = (char *) "-Djava.class.path=";
    options[1].optionString = (char *) "-Djava.library.path=";
    options[2].optionString = (char *) "-verbose:jni";
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 3;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;
    std::cout << "Starting VM..." << std::endl;
    createJavaVM(&jvm, (void **) &env, &vm_args);
    std::cout << "VM started" << std::endl;

    delete[] options;
}

void Jvm::destroy() {
    jvm->DestroyJavaVM();
}

void Jvm::loadJvmDll(CreateJavaVM *createJavaVM) {
#ifdef __linux__
    const char *libPath = "jre/lib/amd64/server/libjvm.so";
    auto jvmLib = dlopen(libPath, RTLD_NOW);
#elif _WIN32
    const char *libPath = "jre/bin/server/jvm.dll";
    HINSTANCE jvmLib = LoadLibrary(libPath);
#else

#endif

    if (jvmLib == nullptr) {
#ifdef __linux__
        std::cout << "Failed to load libjvm.so!" << std::endl;
#elif _WIN32
        DWORD lastErrorCode = GetLastError();
            if (lastErrorCode == 126) {
                // "The specified module could not be found."
                // load msvcr100.dll from the bundled JRE, then try again
                std::cout << "Failed to load jvm.dll. Trying to load msvcr100.dll first ..." << std::endl;

                HINSTANCE hinstVCR = LoadLibrary("jre\\bin\\msvcr100.dll");
                if (hinstVCR != nullptr) {
                    jvmDll = LoadLibrary(jvmDLLPath);
                }
            }
            printf("Error: %d\n", lastErrorCode);
#endif
    }

#ifdef __linux__
    *createJavaVM = (CreateJavaVM) dlsym(jvmLib, "JNI_CreateJavaVM");
#elif _WIN32
    *createJavaVM = (CreateJavaVM) GetProcAddress(jvmLib, "JNI_CreateJavaVM");
    #else
#endif
}
