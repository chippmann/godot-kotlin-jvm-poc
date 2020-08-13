package godot.test

import godot.core.Object

class TestClass: Object() {

    fun _onInit() {
        println("_onInit called!")
    }

    fun _ready() {
        println("_ready called!")
    }

    fun _onDestroy() {
        println("_onDestroy called!")
    }
}