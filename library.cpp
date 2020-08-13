#include "library.h"
#include "jvm/Jvm.h"
#include "Bridge.h"
#include "registration/ClassHandle.h"
#include <iostream>
#include <vector>
#include <jni.h>

#include <gdnative_api_struct.gen.h>

void testBuiltinMethod();
void setupClassLoader(const char *classPath1);
jclass getClass(const char* fqname);
jclass createClass(jclass clazz);

godot_gdnative_ext_nativescript_api_struct *nativescript;
_jobject* urlClassLoader;
jmethodID loadClassMethodId;

void godot_gdnative_init(godot_gdnative_init_options *options) {
    std::cout << "gdnative init called" << std::endl;
    auto gdnative = options->api_struct;
    auto extensionCount = gdnative->num_extensions;
    auto extensions = gdnative->extensions;

    for (int i = 0; i < extensionCount; ++i) {
        auto extension = extensions[i];
        auto type = extension->type;
        if (type == 1) {
            nativescript = (godot_gdnative_ext_nativescript_api_struct*) (extension);
        }
    }
    Jvm::setup();
}

void godot_gdnative_terminate(godot_gdnative_terminate_options *options) {
    std::cout << "gdnative terminate called" << std::endl;
    Jvm::destroy();
}

void godot_nativescript_init(void *handle) {
    std::cout << "nativescript init called" << std::endl;
    setupClassLoader("java/build/libs/java-0.0.1.jar");

    godot_instance_create_func create = {};
    create.create_func = Bridge::createInstance;
    create.method_data = new ClassHandle<jclass>(handle, "", "", []{ return createClass(getClass("godot.test.TestClass")); }, false);
    godot_instance_destroy_func destroy = {};
    destroy.destroy_func = Bridge::destroyInstance;

    nativescript->godot_nativescript_register_class(handle, "TestClass", "Node", create, destroy);

    godot_instance_method method = {};
    method.method_data = (void *) new std::pair<const char*, const char*>("_ready", "()V");
    method.method = Bridge::invokeMethod;
    nativescript->godot_nativescript_register_method(handle, "TestClass", "_ready", {GODOT_METHOD_RPC_MODE_DISABLED}, method);

    testBuiltinMethod();
    auto clazz = getClass("godot.test.TestClass");
    auto instance = createClass(clazz);
    std::cout << "Instance: " << instance << std::endl;
    auto methodID = Jvm::env->GetMethodID(clazz, "_ready", "()V");
    auto blubb = Jvm::env->GetObjectClass(instance);
    auto test = Jvm::env->GetMethodID(blubb, "_ready", "()V");
    std::cout << "methodID: " << instance << std::endl;
    Jvm::env->CallVoidMethod(instance, methodID);
}

void godot_nativescript_terminate(void *handle) {
    std::cout << "nativescript terminate called" << std::endl;
}

void testBuiltinMethod() {
    std::printf("Loading class\n");
    jclass class_System = Jvm::env->FindClass("java/lang/System");
    jfieldID field_System_out = Jvm::env->GetStaticFieldID(class_System, "out", "Ljava/io/PrintStream;");
    _jobject* object_System_out = Jvm::env->GetStaticObjectField(class_System, field_System_out);
    jclass class_PrintStream = Jvm::env->FindClass("java/io/PrintStream");
    jmethodID method_println = Jvm::env->GetMethodID(class_PrintStream, "println", "(I)V");
    Jvm::env->CallObjectMethod(object_System_out, method_println, 777);
    std::cout << "Method Called" << std::endl;
}

jclass createClass(jclass clazz) {
    jmethodID classCtor = Jvm::env->GetMethodID(
            clazz,
            "<init>",
            "()V"
    );
    return (jclass) Jvm::env->NewObject(clazz, classCtor);
}

jclass getClass(const char* fqname) {
    jstring classNameUTF = Jvm::env->NewStringUTF(fqname);
    std::cout << "classNameUTF: " << classNameUTF << std::endl;
    return (jclass) Jvm::env->CallObjectMethod(urlClassLoader, loadClassMethodId, classNameUTF);
}

void setupClassLoader(const char *classPath1) {
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