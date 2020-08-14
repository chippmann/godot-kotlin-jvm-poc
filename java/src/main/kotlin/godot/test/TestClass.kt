package godot.test

import godot.core.Object

class TestClass: Object() {

    fun _onInit() {
        println("_onInit called!")
    }

    fun _ready() {
        println("_ready called!")
    }

    fun _process(delta: Float) {
        println("_process called!")
    }

    fun _onDestroy() {
        println("_onDestroy called!")
    }

    fun benchmark_simple_add() {
        val a = 1
        val b = 2
        val result = a + b
    }

    fun benchmark_avg() {
        val size = 10000
        var total = 0
        for (i in 0 until size) {
            total += i
        }
        val result = total / size
    }
}