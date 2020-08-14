package godot.test

import godot.core.Object

class Simple: Object() {

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

    fun benchmark_simple_add(): Int {
        val a = 1
        val b = 2
        return a + b
    }

    fun benchmark_avg(): Int {
        val size = 10000
        var total = 0
        for (i in 0 until size) {
            total += i
        }
        return total / size
    }
}