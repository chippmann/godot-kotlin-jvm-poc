//
// Created by cedric on 13.08.20.
//

#include "Bridge.h"
#include "registration/ClassHandle.h"
#include "jvm/Jvm.h"
#include "Godot.h"
#include "jni/JniHelper.h"
#include <jni.h>
#include <cstdlib>
#include <string>

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

    std::string signature = methodDataPair->second;
    if (signature == "()I") {
        auto result = (jint) Jvm::env->CallIntMethod(javaInstance, methodId);
        auto variantPtr = std::malloc(sizeof(godot_variant));
        Godot::gdnative->godot_variant_new_int((godot_variant *) (variantPtr), result);
        return *((godot_variant *) variantPtr);
    } else if (signature == "()Lgodot/core/Vector3;"){
        auto result = Jvm::env->CallObjectMethod(javaInstance, methodId);
        if (Jvm::env->ExceptionCheck()) {
            Jvm::env->ExceptionDescribe();
        }
        if (!result) {
            auto variantPtr = std::malloc(sizeof(godot_variant));
            Godot::gdnative->godot_variant_new_nil((godot_variant *) (variantPtr));
            return *((godot_variant *) variantPtr);
        }
        auto vector3Class = JniHelper::getClass("godot.core.Vector3");
        auto xFieldId = Jvm::env->GetFieldID(vector3Class, "x", "D");
        auto yFieldId = Jvm::env->GetFieldID(vector3Class, "y", "D");
        auto zFieldId = Jvm::env->GetFieldID(vector3Class, "z", "D");
        auto x = Jvm::env->GetDoubleField(result, xFieldId);
        auto y = Jvm::env->GetDoubleField(result, yFieldId);
        auto z = Jvm::env->GetDoubleField(result, zFieldId);

        auto vector3Ptr = std::malloc(sizeof(godot_variant));
        Godot::gdnative->godot_vector3_new((godot_vector3 *)vector3Ptr, x, y, z);

        auto variantPtr = std::malloc(sizeof(godot_variant));
        Godot::gdnative->godot_variant_new_vector3((godot_variant *) (variantPtr), (godot_vector3 *) vector3Ptr);
        return *((godot_variant *) variantPtr);
    }

    auto variantPtr = std::malloc(sizeof(godot_variant));
    Godot::gdnative->godot_variant_new_nil((godot_variant *) (variantPtr));
    return *((godot_variant *) variantPtr);
}
