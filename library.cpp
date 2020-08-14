#include "library.h"
#include "src/jvm/Jvm.h"
#include "src/Bridge.h"
#include "src/registration/ClassHandle.h"
#include "src/jni/JniHelper.h"
#include "src/Godot.h"
#include <iostream>
#include <vector>
#include <jni.h>

#include <gdnative_api_struct.gen.h>

void godot_gdnative_init(godot_gdnative_init_options *options) {
    std::cout << "gdnative init called" << std::endl;
    Godot::init(options);
    Jvm::setup();
//    JniHelper::setupClassLoader("java/build/libs/java-0.0.1.jar");
}

void godot_gdnative_terminate(godot_gdnative_terminate_options *options) {
    std::cout << "gdnative terminate called" << std::endl;
    Jvm::destroy();
}

void godot_nativescript_init(void *handle) {
    std::cout << "nativescript init called" << std::endl;

    godot_instance_create_func create = {};
    create.create_func = Bridge::createInstance;
    create.method_data = new ClassHandle<jclass>(handle, "Simple", "Node", []{ return JniHelper::getAnCreateClass("godot.test.Simple"); }, false);
    godot_instance_destroy_func destroy = {};
    destroy.destroy_func = Bridge::destroyInstance;

    Godot::nativescript->godot_nativescript_register_class(handle, "Simple", "Node", create, destroy);

    godot_instance_method method = {};
    method.method_data = (void *) new std::pair<const char*, const char*>("benchmark_simple_add", "()I");
    method.method = Bridge::invokeMethod;
    method.free_func = Godot::gdnative->godot_free;
    Godot::nativescript->godot_nativescript_register_method(handle, "Simple", "benchmark_simple_add", {GODOT_METHOD_RPC_MODE_DISABLED}, method);

    godot_instance_method method2 = {};
    method2.method_data = (void *) new std::pair<const char*, const char*>("benchmark_avg", "()I");
    method2.method = Bridge::invokeMethod;
    method2.free_func = Godot::gdnative->godot_free;
    Godot::nativescript->godot_nativescript_register_method(handle, "Simple", "benchmark_avg", {GODOT_METHOD_RPC_MODE_DISABLED}, method2);
}

void godot_nativescript_terminate(void *handle) {
    std::cout << "nativescript terminate called" << std::endl;
}