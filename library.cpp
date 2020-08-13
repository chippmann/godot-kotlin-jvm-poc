#include "library.h"
#include "jvm/Jvm.h"
#include "Bridge.h"
#include "registration/ClassHandle.h"
#include "jni/JniHelper.h"
#include "Godot.h"
#include <iostream>
#include <vector>
#include <jni.h>

#include <gdnative_api_struct.gen.h>

void godot_gdnative_init(godot_gdnative_init_options *options) {
    std::cout << "gdnative init called" << std::endl;
    Godot::init(options);
    Jvm::setup();
}

void godot_gdnative_terminate(godot_gdnative_terminate_options *options) {
    std::cout << "gdnative terminate called" << std::endl;
    Jvm::destroy();
}

void godot_nativescript_init(void *handle) {
    std::cout << "nativescript init called" << std::endl;
    JniHelper::setupClassLoader("java/build/libs/java-0.0.1.jar");

    godot_instance_create_func create = {};
    create.create_func = Bridge::createInstance;
    create.method_data = new ClassHandle<jclass>(handle, "TestClass", "Node", []{ return JniHelper::getAnCreateClass("godot.test.TestClass"); }, false);
    godot_instance_destroy_func destroy = {};
    destroy.destroy_func = Bridge::destroyInstance;

    Godot::nativescript->godot_nativescript_register_class(handle, "TestClass", "Node", create, destroy);

    godot_instance_method method = {};
    method.method_data = (void *) new std::pair<const char*, const char*>("_ready", "()V");
    method.method = Bridge::invokeMethod;
    method.free_func = Godot::gdnative->godot_free;
    Godot::nativescript->godot_nativescript_register_method(handle, "TestClass", "_ready", {GODOT_METHOD_RPC_MODE_DISABLED}, method);
}

void godot_nativescript_terminate(void *handle) {
    std::cout << "nativescript terminate called" << std::endl;
}