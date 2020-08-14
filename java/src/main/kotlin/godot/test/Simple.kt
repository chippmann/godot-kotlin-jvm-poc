package godot.test

import godot.core.Basis
import godot.core.Object
import godot.core.Transform
import godot.core.Vector3

const val PI = kotlin.math.PI
const val TAU = 2 * PI

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

    fun benchmark_vectors(): Vector3 {
        var b = Transform()
        b = b.rotated(Vector3.UP, deg2rad(60.0))
        b = b.scaled(Vector3(0.5, 0.5, 0.5))

        var s = Vector3()
        for (i in 0 until 1000) {
            val iDouble = i.toDouble()
            var v = Vector3(iDouble, iDouble, iDouble)
            v = b.xform(v)
            s += v
        }

        return s
    }

    /** Returns degrees converted to radians. */
    private fun deg2rad(s: Double) = s * TAU / 360
}