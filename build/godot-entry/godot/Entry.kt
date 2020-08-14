// THIS FILE IS GENERATED! DO NOT EDIT IT MANUALLY! ALL CHANGES TO IT WILL BE OVERWRITTEN ON EACH BUILD
@file:Suppress("EXPERIMENTAL_API_USAGE")

package godot

import Simple
import godot.MultiplayerAPI.RPCMode.DISABLED
import godot.core.ClassRegistry
import godot.core.Godot
import godot.gdnative.godot_gdnative_init_options
import godot.gdnative.godot_gdnative_terminate_options
import kotlin.Int
import kotlin.Suppress
import kotlin.native.CName
import kotlinx.cinterop.COpaquePointer

@CName("godot_gdnative_init")
fun GDNativeInit(options: godot_gdnative_init_options) {
  Godot.init(options)
}

@CName("godot_gdnative_terminate")
fun GDNativeTerminate(options: godot_gdnative_terminate_options) {
  Godot.terminate(options)
}

@CName("godot_nativescript_init")
fun NativeScriptInit(handle: COpaquePointer) {
  Godot.nativescriptInit(handle)
  with(ClassRegistry(handle)) {
    registerClass("Simple", "Object", ::Simple, false) {
      function("benchmarkSimpleAdd", DISABLED, Simple::benchmarkSimpleAdd, getTypeToVariantConversionFunction<Int>())
      function("benchmarkAvg", DISABLED, Simple::benchmarkAvg, getTypeToVariantConversionFunction<Int>())
    }
  }
}

@CName("godot_nativescript_terminate")
fun NativeScriptTerminate(handle: COpaquePointer) {
  Godot.nativescriptTerminate(handle)
}
