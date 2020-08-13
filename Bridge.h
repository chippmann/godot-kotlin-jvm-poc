//
// Created by cedric on 13.08.20.
//

#ifndef TESTINTEROP_BRIDGE_H
#define TESTINTEROP_BRIDGE_H


#include <gdnative/variant.h>

class Bridge {
public:
    static void* createInstance(void *instance, void *methodData);
    static void destroyInstance(void *instance, void *methodData, void *classData);
    static godot_variant invokeMethod(void *instance, void *methodData, void *classData, int numArgs, godot_variant **args);
};


#endif //TESTINTEROP_BRIDGE_H
