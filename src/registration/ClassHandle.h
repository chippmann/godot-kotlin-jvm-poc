//
// Created by cedric on 13.08.20.
//

#ifndef TESTINTEROP_CLASSHANDLE_H
#define TESTINTEROP_CLASSHANDLE_H


#include <functional>
#include "../jvm/Jvm.h"

template<class T>
class ClassHandle {
public:
    ClassHandle(
            void *nativeScriptHandle,
            const char *className,
            const char *parentClassName,
            std::function<T()> factory,
            bool isTool
    );
public:
    T wrap(void *instance);

private:
    void *nativeScriptHandle;
    const char *className;
    const char *parentClassName;
    std::function<T()> factory;
    const bool isTool;
};

template<class T>
ClassHandle<T>::ClassHandle(
        void *nativeScriptHandle,
        const char *className,
        const char *parentClassName,
        std::function<T()> factory,
        const bool isTool
): nativeScriptHandle(nativeScriptHandle), className(className), parentClassName(parentClassName), factory(factory), isTool(isTool){}

template<class T>
T ClassHandle<T>::wrap(void *instance) {
    auto javaInstance = (jclass) this->factory();
    auto fieldId = Jvm::env->GetFieldID(Jvm::env->GetObjectClass(javaInstance), "_ptr", "J");
    Jvm::env->SetLongField(javaInstance, fieldId, (long) instance);
    return (T) javaInstance;
}


#endif //TESTINTEROP_CLASSHANDLE_H
