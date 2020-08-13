//
// Created by cedric on 13.08.20.
//

#ifndef TESTINTEROP_GODOT_H
#define TESTINTEROP_GODOT_H


#include <gdnative/gdnative.h>
#include <gdnative_api_struct.gen.h>

class Godot {
public:
    static const godot_gdnative_core_api_struct *gdnative;
    static godot_gdnative_ext_nativescript_api_struct *nativescript;
    static void init(godot_gdnative_init_options *options);
};


#endif //TESTINTEROP_GODOT_H
