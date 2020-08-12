#ifndef TESTINTEROP_LIBRARY_H
#define TESTINTEROP_LIBRARY_H

#include "godot-headers/gdnative/gdnative.h"

extern "C" void godot_gdnative_init(godot_gdnative_init_options *options);
extern "C" void godot_gdnative_terminate(godot_gdnative_terminate_options *options);
extern "C" void godot_nativescript_init(void * handle);
extern "C" void godot_nativescript_terminate(void * handle);


#endif //TESTINTEROP_LIBRARY_H
