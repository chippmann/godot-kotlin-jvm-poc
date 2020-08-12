#include "library.h"
#include <iostream>
#include <vector>
#include <jni.h>

#ifdef __linux__
#include <dlfcn.h>
#include <gdnative_api_struct.gen.h>

#elif _WIN32
#include <Windows.h>
#else

#endif

typedef jint(JNICALL *CreateJavaVM)(JavaVM **, void **, void *);
void loadJvmDll(CreateJavaVM *createJavaVM);
void createJVM();
void testBuiltinMethod();
int testCustomJavaMethod(const char *classPath1, const char *className, const char *methodName);


JavaVM *jvm;
JNIEnv_ *env;

godot_gdnative_ext_nativescript_api_struct *nativescript;

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
    createJVM();
}

void godot_gdnative_terminate(godot_gdnative_terminate_options *options) {
    std::cout << "gdnative terminate called" << std::endl;
    jvm->DestroyJavaVM();
}

void *_godot_class_instance_func(godot_object *p, void *method_data) {
    void *t = {};
    return t;
}

void _godot_class_destroy_func(godot_object *p, void *method_data, void *data) {
}

godot_variant _godot_func(godot_object *, void *, void *, int, godot_variant **) {
    testCustomJavaMethod("java", "TestClass", "_ready");
    return {};
}

void godot_nativescript_init(void *handle) {
    std::cout << "nativescript init called" << std::endl;

    godot_instance_create_func create = {};
    create.create_func = _godot_class_instance_func;
    godot_instance_destroy_func destroy = {};
    destroy.destroy_func = _godot_class_destroy_func;

    nativescript->godot_nativescript_register_class(handle, "TestClass", "Node", create, destroy);

    godot_instance_method method = {};
    method.method = _godot_func;
    nativescript->godot_nativescript_register_method(handle, "TestClass", "ready", {GODOT_METHOD_RPC_MODE_DISABLED}, method);

    testBuiltinMethod();
    testCustomJavaMethod("java", "TestClass", "ready");
}

void godot_nativescript_terminate(void *handle) {
    std::cout << "nativescript terminate called" << std::endl;
}

void testBuiltinMethod() {
    std::printf("Loading class\n");
    jclass class_System = env->FindClass("java/lang/System");
    jfieldID field_System_out = env->GetStaticFieldID(class_System, "out", "Ljava/io/PrintStream;");
    _jobject* object_System_out = env->GetStaticObjectField(class_System, field_System_out);
    jclass class_PrintStream = env->FindClass("java/io/PrintStream");
    jmethodID method_println = env->GetMethodID(class_PrintStream, "println", "(I)V");
    env->CallObjectMethod(object_System_out, method_println, 777);
    std::cout << "Method Called" << std::endl;
}

int testCustomJavaMethod(const char *classPath1, const char *className, const char *methodName) {
    std::vector<std::string> classPath;
    classPath.emplace_back(classPath1);

    jclass urlClass = env->FindClass("java/net/URL");
    jobjectArray urlArray = env->NewObjectArray(classPath.size(), urlClass, nullptr);

    size_t i = 0;
    for (const std::string &classPathURL : classPath) {
        jstring urlStr = env->NewStringUTF(classPathURL.c_str());
        jclass fileClass = env->FindClass("java/io/File");
        jmethodID fileCtor = env->GetMethodID(fileClass, "<init>", "(Ljava/lang/String;)V");
        _jobject* file = env->NewObject(fileClass, fileCtor, urlStr);
        jmethodID toUriMethod = env->GetMethodID(fileClass, "toURI", "()Ljava/net/URI;");
        _jobject* uri = env->CallObjectMethod(file, toUriMethod);
        jclass uriClass = env->FindClass("java/net/URI");
        jmethodID toUrlMethod = env->GetMethodID(uriClass, "toURL", "()Ljava/net/URL;");
        _jobject* url = env->CallObjectMethod(uri, toUrlMethod);

        env->SetObjectArrayElement(urlArray, i++, url);
    }

    jclass threadClass = env->FindClass("java/lang/Thread");
    jmethodID threadGetCurrent = env->GetStaticMethodID(threadClass, "currentThread", "()Ljava/lang/Thread;");
    _jobject* thread = env->CallStaticObjectMethod(threadClass, threadGetCurrent);
    jmethodID threadGetLoader = env->GetMethodID(threadClass, "getContextClassLoader", "()Ljava/lang/ClassLoader;");
    _jobject* contextClassLoader = env->CallObjectMethod(thread, threadGetLoader);
    jclass urlClassLoaderClass = env->FindClass("java/net/URLClassLoader");
    jmethodID urlClassLoaderCtor = env->GetMethodID(
            urlClassLoaderClass,
            "<init>",
            "([Ljava/net/URL;Ljava/lang/ClassLoader;)V"
    );
    _jobject* urlClassLoader = env->NewObject(urlClassLoaderClass, urlClassLoaderCtor, urlArray, contextClassLoader);
    jmethodID threadSetLoader = env->GetMethodID(threadClass, "setContextClassLoader", "(Ljava/lang/ClassLoader;)V");
    env->CallVoidMethod(thread, threadSetLoader, urlClassLoader);
    jmethodID loadClass = env->GetMethodID(urlClassLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    jstring mainClassNameUTF = env->NewStringUTF(className);
    _jobject* mainClass = env->CallObjectMethod(urlClassLoader, loadClass, mainClassNameUTF);




    jmethodID mainClassCtor = env->GetMethodID(
            (jclass) mainClass,
            "<init>",
            "()V"
    );
    std::cout << "mainClassCtor: " << mainClassCtor << std::endl;
    _jobject* t = env->NewObject((jclass) mainClass, mainClassCtor);
    std::cout << "t: " << t << std::endl;
    jmethodID ready = env->GetMethodID((jclass) mainClass, "ready", "()V");
    std::cout << "ready: " << ready << std::endl;
    env->CallVoidMethod(t, ready);

    return 0;
}

void createJVM() {
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

void loadJvmDll(CreateJavaVM *createJavaVM) {
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