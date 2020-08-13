//
// Created by cedric on 13.08.20.
//

#include "Godot.h"

godot_gdnative_ext_nativescript_api_struct *Godot::nativescript = nullptr;
const godot_gdnative_core_api_struct *Godot::gdnative = nullptr;

void Godot::init(godot_gdnative_init_options *options) {
    gdnative = options->api_struct;
    auto extensionCount = gdnative->num_extensions;
    auto extensions = gdnative->extensions;

    for (int i = 0; i < extensionCount; ++i) {
        auto extension = extensions[i];
        auto type = extension->type;
        if (type == 1) {
            nativescript = (godot_gdnative_ext_nativescript_api_struct*) (extension);
        }
    }
}
