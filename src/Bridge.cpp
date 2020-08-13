//
// Created by cedric on 13.08.20.
//

#include "Bridge.h"
#include "registration/ClassHandle.h"
#include "jvm/Jvm.h"
#include "Godot.h"
#include <jni.h>
#include <cstdlib>

void *Bridge::createInstance(void *instance, void *methodData) {
    auto *classHandle = (ClassHandle<jclass> *) methodData;
    auto javaInstance = classHandle->wrap(instance);
    auto methodId = Jvm::env->GetMethodID(Jvm::env->GetObjectClass(javaInstance), "_onInit", "()V");
    Jvm::env->CallVoidMethod(javaInstance, methodId);
    return Jvm::env->NewGlobalRef(javaInstance);
}

void Bridge::destroyInstance(void *instance, void *methodData, void *classData) {
    auto javaInstance = (jclass) classData;
    auto methodId = Jvm::env->GetMethodID(Jvm::env->GetObjectClass(javaInstance), "_onDestroy", "()V");
    Jvm::env->CallVoidMethod(javaInstance, methodId);
    Jvm::env->DeleteGlobalRef(javaInstance);
}

godot_variant Bridge::invokeMethod(godot_object *instance, void *methodData, void *classData, int numArgs, godot_variant **args) {
    auto javaInstance = (jclass) classData;
    auto methodDataPair = (std::pair<char *, char *> *) methodData;
    auto objectClass = Jvm::env->GetObjectClass(javaInstance);
    auto methodId = Jvm::env->GetMethodID(objectClass, methodDataPair->first, methodDataPair->second);
//    auto variantJavaInstance = (jclass) Jvm::env->CallObjectMethod(javaInstance, methodId, args);

    Jvm::env->CallVoidMethod(javaInstance, methodId);


    auto variantPtr = std::malloc(sizeof(godot_variant));
    Godot::gdnative->godot_variant_new_nil((godot_variant *) (variantPtr));
    return *((godot_variant *) variantPtr);
}
