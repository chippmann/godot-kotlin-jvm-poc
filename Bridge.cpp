//
// Created by cedric on 13.08.20.
//

#include "Bridge.h"
#include "registration/ClassHandle.h"
#include "jvm/Jvm.h"
#include <jni.h>

void *Bridge::createInstance(void *instance, void *methodData) {
    auto* classHandle = (ClassHandle<jclass>*) methodData;
    auto javaInstance = classHandle->wrap(instance);
    auto methodId = Jvm::env->GetMethodID(javaInstance, "_onInit", "()V");
    Jvm::env->CallVoidMethod(javaInstance, methodId);
    return Jvm::env->NewGlobalRef(javaInstance);
}

void Bridge::destroyInstance(void *instance, void *methodData, void *classData) {
    auto javaInstance = (jclass) &classData;
    auto methodId = Jvm::env->GetMethodID(javaInstance, "_onDestroy", "()V");
    Jvm::env->CallVoidMethod(javaInstance, methodId);
    Jvm::env->DeleteGlobalRef(javaInstance);
}

godot_variant Bridge::invokeMethod(void *instance, void *methodData, void *classData, int numArgs, godot_variant **args) {
    auto javaInstance = (jclass) classData;
    auto methodDataPair = (std::pair<char*, char*>*) methodData;
    auto methodId = Jvm::env->GetMethodID(javaInstance, methodDataPair->first, methodDataPair->second);
    auto variantJavaInstance = (jclass) Jvm::env->CallObjectMethod(javaInstance, methodId, args);
    auto variantHandleFieldId = Jvm::env->GetFieldID(variantJavaInstance, "_handle", "L");
    auto ptr = ((godot_variant *) Jvm::env->GetLongField(variantJavaInstance, variantHandleFieldId));
    return *ptr;
}
