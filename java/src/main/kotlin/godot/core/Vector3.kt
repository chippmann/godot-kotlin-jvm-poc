package godot.core

import kotlin.math.atan2
import kotlin.math.sqrt

class Vector3(
    var x: Double,
    var y: Double,
    var z: Double
) {

    //CONSTANTS
    enum class Axis(val value: Long) {
        X(0),
        Y(1),
        Z(2);

        companion object {
            @JvmStatic
            fun from(value: Long) = when (value) {
                0L -> X
                1L -> Y
                2L -> Z
                else -> throw AssertionError("Unknown axis for Vector3: $value")
            }
        }
    }

    companion object {
        @JvmStatic
        val AXIS_X = Axis.X.value
        @JvmStatic
        val AXIS_Y = Axis.Y.value
        @JvmStatic
        val AXIS_Z = Axis.Z.value
        @JvmStatic
        val ZERO: Vector3
            get() = Vector3(0.0, 0.0, 0.0)
        @JvmStatic
        val ONE: Vector3
            get() = Vector3(1.0, 1.0, 1.0)
        @JvmStatic
        val INF: Vector3
            get() = Vector3(Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY)
        @JvmStatic
        val LEFT: Vector3
            get() = Vector3(-1.0, 0.0, 0.0)
        @JvmStatic
        val RIGHT: Vector3
            get() = Vector3(1.0, 0.0, 0.0)
        @JvmStatic
        val UP: Vector3
            get() = Vector3(0.0, 1.0, 0.0)
        @JvmStatic
        val DOWN: Vector3
            get() = Vector3(0.0, -1.0, 0.0)
        @JvmStatic
        val FORWARD: Vector3
            get() = Vector3(0.0, 0.0, -1.0)
        @JvmStatic
        val BACK: Vector3
            get() = Vector3(0.0, 0.0, 1.0)
    }



    constructor(vec: Vector3) :
            this(vec.x, vec.y, vec.z)

    constructor(): this(0.0, 0.0, 0.0)


    //API
    /**
     * Returns a new vector with all components in absolute values (i.e. positive).
     */

    /**
     * Returns the minimum angle to the given vector.
     */
    fun angleTo(to: Vector3): Double {
        return atan2(cross(to).length(), dot(to))
    }


    /**
     * Returns a new vector with all components rounded up.
     */
    fun ceil(): Vector3 {
        return Vector3(kotlin.math.ceil(x), kotlin.math.ceil(y), kotlin.math.ceil(z))
    }

    /**
     * Returns the cross product with b.
     */
    fun cross(b: Vector3): Vector3 {
        return Vector3((y * b.z) - (z * b.y), (z * b.x) - (x * b.z), (x * b.y) - (y * b.x))
    }



    /**
     * Returns the squared distance to b.
     * Prefer this function over distance_to if you need to sort vectors or need the squared distance for some formula.
     */
    fun distanceSquaredTo(other: Vector3): Double {
        return (other - this).lengthSquared()
    }

    /**
     * Returns the distance to b.
     */
    fun distanceTo(other: Vector3): Double {
        return (other - this).length()
    }

    /**
     * Returns the dot product with b.
     */
    fun dot(b: Vector3): Double {
        return x * b.x + y * b.y + z * b.z
    }

    /**
     * Returns a new vector with all components rounded down.
     */
    fun floor(): Vector3 {
        return Vector3(kotlin.math.floor(x), kotlin.math.floor(y), kotlin.math.floor(z))
    }




    /**
     * Returns the vector’s length.
     */
    fun length(): Double {
        return sqrt(x * x + y * y + z * z)
    }

    /**
     * Returns the vector’s length squared.
     * Prefer this function over length if you need to sort vectors or need the squared length for some formula.
     */
    fun lengthSquared(): Double {
        return x * x + y * y + z * z
    }

    /**
     * Returns the result of the linear interpolation between this vector and b by amount t.
     * t is in the range of 0.0 - 1.0, representing the amount of interpolation.
     */
    fun linearInterpolate(b: Vector3, t: Double): Vector3 {
        return Vector3(x + (t * (b.x - x)), y + (t * (b.y - y)), z + (t * (b.z - z)))
    }

    /**
     * Returns the axis of the vector’s largest value. See AXIS_* constants.
     */
    fun maxAxis(): Int {
        return if (x < y) {
            if (y < z) {
                2
            } else {
                1
            }
        } else {
            if (x < z) {
                2
            } else {
                0
            }
        }
    }

    /**
     * Returns the axis of the vector’s smallest value. See AXIS_* constants.
     */
    fun minAxis(): Int {
        return if (x < y) {
            if (x < z) {
                0
            } else {
                2
            }
        } else {
            if (y < z) {
                1
            } else {
                2
            }
        }
    }




    /**
     * Returns a vector composed of the fposmod of this vector’s components and mod.
     */
    fun posmod(mod: Double): Vector3 {
        return Vector3(x.rem(mod), y.rem(mod), z.rem(mod))
    }

    /**
     * Returns a vector composed of the fposmod of this vector’s components and modv’s components.
     */
    fun posmodv(modv: Vector3): Vector3 {
        return Vector3(x.rem(modv.x), y.rem(modv.y), z.rem(modv.z))
    }

    /**
     * Returns the vector projected onto the vector b.
     */
    fun project(vec: Vector3): Vector3 {
        val v1: Vector3 = vec
        val v2: Vector3 = this
        return v2 * (v1.dot(v2) / v2.dot(v2))
    }




    /**
     * Returns the vector with all components rounded to the nearest integer, with halfway cases rounded away from zero.
     */
    fun round(): Vector3 {
        return Vector3(kotlin.math.round(x), kotlin.math.round(y), kotlin.math.round(z))
    }

    /**
     * Returns the vector with each component set to one or negative one, depending on the signs of the components.
     */
    fun sign(): Vector3 {
        return Vector3(kotlin.math.sign(x), kotlin.math.sign(y), kotlin.math.sign(z))
    }


    /**
     * Returns the component of the vector along a plane defined by the given normal.
     */
    fun slide(vec: Vector3): Vector3 {
        return vec - this * this.dot(vec)
    }





    //UTILITIES

    operator fun get(n: Int): Double =
        when (n) {
            0 -> x
            1 -> y
            2 -> z
            else -> throw IndexOutOfBoundsException()
        }

    operator fun set(n: Int, f: Double): Unit =
        when (n) {
            0 -> x = f
            1 -> y = f
            2 -> z = f
            else -> throw IndexOutOfBoundsException()
        }

    operator fun plus(vec: Vector3) = Vector3(x + vec.x, y + vec.y, z + vec.z)
    operator fun plus(scalar: Int) = Vector3(x + scalar, y + scalar, z + scalar)
    operator fun plus(scalar: Long) = Vector3(x + scalar, y + scalar, z + scalar)
    operator fun plus(scalar: Double) = Vector3(x + scalar, y + scalar, z + scalar)

    operator fun minus(vec: Vector3) = Vector3(x - vec.x, y - vec.y, z - vec.z)
    operator fun minus(scalar: Int) = Vector3(x - scalar, y - scalar, z - scalar)
    operator fun minus(scalar: Long) = Vector3(x - scalar, y - scalar, z - scalar)
    operator fun minus(scalar: Double) = Vector3(x - scalar, y - scalar, z - scalar)

    operator fun times(vec: Vector3) = Vector3(x * vec.x, y * vec.y, z * vec.z)
    operator fun times(scalar: Int) = Vector3(x * scalar, y * scalar, z * scalar)
    operator fun times(scalar: Long) = Vector3(x * scalar, y * scalar, z * scalar)
    operator fun times(scalar: Double) = Vector3(x * scalar, y * scalar, z * scalar)

    operator fun div(vec: Vector3) = Vector3(x / vec.x, y / vec.y, z / vec.z)
    operator fun div(scalar: Int) = Vector3(x / scalar, y / scalar, z / scalar)
    operator fun div(scalar: Long) = Vector3(x / scalar, y / scalar, z / scalar)
    operator fun div(scalar: Double) = Vector3(x / scalar, y / scalar, z / scalar)

    operator fun unaryMinus() = Vector3(-x, -y, -z)

    override fun equals(other: Any?): Boolean =
        when (other) {
            is Vector3 -> (x == other.x && y == other.y && z == other.z)
            else -> false
        }

    override fun toString(): String {
        return "($x, $y, $z)"
    }

    override fun hashCode(): Int {
        return this.toString().hashCode()
    }
}

operator fun Int.plus(vec: Vector3) = vec + this
operator fun Long.plus(vec: Vector3) = vec + this
operator fun Double.plus(vec: Vector3) = vec + this

operator fun Int.minus(vec: Vector3) = Vector3(this - vec.x, this - vec.y, this - vec.z)
operator fun Long.minus(vec: Vector3) = Vector3(this - vec.x, this - vec.y, this - vec.z)
operator fun Double.minus(vec: Vector3) = Vector3(this - vec.x, this - vec.y, this - vec.z)

operator fun Int.times(vec: Vector3) = vec * this
operator fun Long.times(vec: Vector3) = vec * this
operator fun Double.times(vec: Vector3) = vec * this