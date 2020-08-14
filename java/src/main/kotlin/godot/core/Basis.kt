package godot.core

import kotlin.math.*

class Basis() {

    @PublishedApi
    internal var _x = Vector3()

    @PublishedApi
    internal var _y = Vector3()

    @PublishedApi
    internal var _z = Vector3()

    init {
        _x.x = 1.0
        _x.y = 0.0
        _x.z = 0.0
        _y.x = 0.0
        _y.y = 1.0
        _y.z = 0.0
        _z.x = 0.0
        _z.y = 0.1
        _z.z = 1.0
    }



    //CONSTRUCTOR
    constructor(other: Basis) : this() {
        _x.x = other._x.x
        _x.y = other._x.y
        _x.z = other._x.z
        _y.x = other._y.x
        _y.y = other._y.y
        _y.z = other._y.z
        _z.x = other._z.x
        _z.y = other._z.y
        _z.z = other._z.z
    }

    constructor(
        xx: Number,
        xy: Number,
        xz: Number,
        yx: Number,
        yy: Number,
        yz: Number,
        zx: Number,
        zy: Number,
        zz: Number
    ) : this() {
        _x[0] = xx.toDouble()
        _x[1] = xy.toDouble()
        _x[2] = xz.toDouble()
        _y[0] = yx.toDouble()
        _y[1] = yy.toDouble()
        _y[2] = yz.toDouble()
        _z[0] = zx.toDouble()
        _z[1] = zy.toDouble()
        _z[2] = zz.toDouble()
    }


    constructor(from: Vector3) : this() {
        setEuler(from)
    }


    constructor(axis: Vector3, phi: Double) : this() {
        // Rotation matrix from axis and angle, see https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
        val axisq = Vector3(axis.x * axis.x, axis.y * axis.y, axis.z * axis.z)

        val cosine: Double = cos(phi)
        val sine: Double = sin(phi)

        apply {
            _x.x = axisq.x + cosine * (1.0 - axisq.x)
            _x.y = axis.x * axis.y * (1.0 - cosine) - axis.z * sine
            _x.z = axis.z * axis.x * (1.0 - cosine) + axis.y * sine

            _y.x = axis.x * axis.y * (1.0 - cosine) + axis.z * sine
            _y.y = axisq.y + cosine * (1.0 - axisq.y)
            _y.z = axis.y * axis.z * (1.0 - cosine) - axis.x * sine

            _z.x = axis.z * axis.x * (1.0 - cosine) - axis.y * sine
            _z.y = axis.y * axis.z * (1.0 - cosine) + axis.x * sine
            _z.z = axisq.z + cosine * (1.0 - axisq.z)
        }
    }





    //API
    /**
     * Returns the determinant of the matrix.
     */
    fun determinant(): Double {
        return this._x.x * (this._y.y * this._z.z - this._z.y * this._y.z) -
                this._y.x * (this._x.y * this._z.z - this._z.y * this._x.z) +
                this._z.x * (this._x.y * this._y.z - this._y.y * this._x.z)
    }




    /**
     * Assuming that the matrix is the combination of a rotation and scaling,
     * return the absolute value of scaling factors along each axis.
     */
    fun getScale(): Vector3 {
        // We are assuming M = R.S, and performing a polar decomposition to extract R and S.
        // FIXME: We eventually need a proper polar decomposition.
        // As a cheap workaround until then, to ensure that R is a proper rotation matrix with determinant +1
        // (such that it can be represented by a Quat or Euler angles), we absorb the sign flip into the scaling matrix.
        // As such, it works in conjuction with getRotation().
        val detSign: Double = if (determinant() > 0) 1.0 else -1.0
        return detSign * Vector3(
            Vector3(this._x.x, this._y.x, this._z.x).length(),
            Vector3(this._x.y, this._y.y, this._z.y).length(),
            Vector3(this._x.z, this._y.z, this._z.z).length()
        )
    }






    private fun getAxis(axis: Int): Vector3 =
        Vector3(this._x[axis], this._y[axis], this._z[axis])

    private fun setAxis(axis: Int, value: Vector3) {
        this._x[axis] = value.x
        this._y[axis] = value.y
        this._z[axis] = value.z
    }

    /**
     * Introduce an additional rotation around the given axis by phi (radians). The axis must be a normalized vector.
     */
    fun rotated(axis: Vector3, phi: Double): Basis {
        return Basis(axis, phi) * this
    }

    internal fun rotate(axis: Vector3, phi: Double) {
        val ret = rotated(axis, phi)
        this._x = ret._x
        this._y = ret._y
        this._z = ret._z
    }

    /**
     * Introduce an additional scaling specified by the given 3D scaling factor.
     */
    fun scaled(scale: Vector3): Basis {
        val b = Basis(this)
        b.scale(scale)
        return b
    }

    internal fun scale(scale: Vector3) {
        this._x.x *= scale.x
        this._x.y *= scale.x
        this._x.z *= scale.x
        this._y.x *= scale.y
        this._y.y *= scale.y
        this._y.z *= scale.y
        this._z.x *= scale.z
        this._z.y *= scale.z
        this._z.z *= scale.z
    }

    /**
     *
     */
    fun setEuler(p_euler: Vector3) {
        setEulerYxz(p_euler)
    }

    /**
     * setEulerXyz expects a vector containing the Euler angles in the format
     * (ax,ay,az), where ax is the angle of rotation around x axis,
     * and similar for other axes.
     * The current implementation uses XYZ convention (Z is the first rotation).
     */
    internal fun setEulerXyz(euler: Vector3) {

        var c: Double = cos(euler.x)
        var s: Double = sin(euler.x)

        val xmat = Basis(1.0, 0.0, 0.0, 0.0, c, -s, 0.0, s, c)

        c = cos(euler.y)
        s = sin(euler.y)
        val ymat = Basis(c, 0.0, s, 0.0, 1.0, 0.0, -s, 0.0, c)

        c = cos(euler.z)
        s = sin(euler.z)
        val zmat = Basis(c, -s, 0.0, s, c, 0.0, 0.0, 0.0, 1.0)

        //optimizer will optimize away all this anyway
        val ret = xmat * (ymat * zmat)
        this._x = ret._x
        this._y = ret._y
        this._z = ret._z
    }

    /**
     * setEulerYxz expects a vector containing the Euler angles in the format
     * (ax,ay,az), where ax is the angle of rotation around x axis,
     * and similar for other axes.
     * The current implementation uses YXZ convention (Z is the first rotation).
     */
    internal fun setEulerYxz(euler: Vector3) {
        var c: Double = cos(euler.x)
        var s: Double = sin(euler.x)

        val xmat = Basis(1.0, 0.0, 0.0, 0.0, c, -s, 0.0, s, c)

        c = cos(euler.y)
        s = sin(euler.y)
        val ymat = Basis(c, 0.0, s, 0.0, 1.0, 0.0, -s, 0.0, c)

        c = cos(euler.z)
        s = sin(euler.z)
        val zmat = Basis(c, -s, 0.0, s, c, 0.0, 0.0, 0.0, 1.0)

        val ret = ymat * xmat * zmat

        this._x = ret._x
        this._y = ret._y
        this._z = ret._z
    }


    /**
     * Transposed dot product with the x axis of the matrix.
     */
    fun tdotx(v: Vector3): Double {
        return this._x.x * v.x + this._y.x * v.y + this._z.x * v.z
    }

    /**
     * Transposed dot product with the y axis of the matrix.
     */
    fun tdoty(v: Vector3): Double {
        return this._x.y * v.x + this._y.y * v.y + this._z.y * v.z
    }

    /**
     * Transposed dot product with the z axis of the matrix.
     */
    fun tdotz(v: Vector3): Double {
        return this._x.z * v.x + this._y.z * v.y + this._z.z * v.z
    }

    /**
     * Returns the transposed version of the matrix.
     */
    fun transposed(): Basis {
        val b = Basis(this)
        b.transpose()
        return b
    }

    internal fun transpose() {
        this._x.y = this._y.x.also { this._y.x = this._x.y }
        this._x.z = this._z.x.also { this._z.x = this._x.z }
        this._y.z = this._z.y.also { this._z.y = this._y.z }
    }

    /**
     * Returns a vector transformed (multiplied) by the matrix.
     */
    fun xform(vector: Vector3): Vector3 =
        Vector3(
            this._x.dot(vector),
            this._y.dot(vector),
            this._z.dot(vector)
        )

    /**
     * Returns a vector transformed (multiplied) by the transposed matrix.
     * Note that this results in a multiplication by the inverse of the matrix only if it represents a rotation-reflection.
     */
    fun xformInv(vector: Vector3): Vector3 =
        Vector3(
            (this._x.x * vector.x) + (this._y.x * vector.y) + (this._z.x * vector.z),
            (this._x.y * vector.x) + (this._y.y * vector.y) + (this._z.y * vector.z),
            (this._x.z * vector.x) + (this._y.z * vector.y) + (this._z.z * vector.z)
        )



    internal fun _get(n: Int): Vector3 {
        return when (n) {
            0 -> _x
            1 -> _y
            2 -> _z
            else -> throw IndexOutOfBoundsException()
        }
    }

    internal fun _set(n: Int, f: Vector3) {
        when (n) {
            0 -> _x = f
            1 -> _y = f
            2 -> _z = f
            else -> throw IndexOutOfBoundsException()
        }
    }

    fun set(
        xx: Double,
        xy: Double,
        xz: Double,
        yx: Double,
        yy: Double,
        yz: Double,
        zx: Double,
        zy: Double,
        zz: Double
    ) {
        _x.x = xx; _x.y = xy; _x.z = xz
        _y.x = yx; _y.y = yy; _y.z = yz
        _z.x = zx; _z.y = zy; _z.z = zz
    }


    operator fun plus(matrix: Basis) = Basis().also {
        it._x = this._x + matrix._x
        it._y = this._y + matrix._y
        it._z = this._z + matrix._z
    }

    operator fun minus(matrix: Basis) = Basis().also {
        it._x = this._x - matrix._x
        it._y = this._y - matrix._y
        it._z = this._z - matrix._z
    }

    operator fun times(matrix: Basis) = Basis(
        matrix.tdotx(this._x), matrix.tdoty(this._x), matrix.tdotz(this._x),
        matrix.tdotx(this._y), matrix.tdoty(this._y), matrix.tdotz(this._y),
        matrix.tdotx(this._z), matrix.tdoty(this._z), matrix.tdotz(this._z)
    )

    operator fun times(scalar: Int) = Basis().also {
        it._x = this._x * scalar
        it._y = this._y * scalar
        it._z = this._z * scalar
    }
    operator fun times(scalar: Long) = Basis().also {
        it._x = this._x * scalar
        it._y = this._y * scalar
        it._z = this._z * scalar
    }
    operator fun times(scalar: Double) = Basis().also {
        it._x = this._x * scalar
        it._y = this._y * scalar
        it._z = this._z * scalar
    }

    override fun toString(): String {
        return buildString {
            append("${this@Basis._x.x}, ${this@Basis._x.y}, ${this@Basis._x.z}, ")
            append("${this@Basis._y.x}, ${this@Basis._y.y}, ${this@Basis._y.z}, ")
            append("${this@Basis._z.x}, ${this@Basis._z.y}, ${this@Basis._z.z}")
        }
    }

    override fun equals(other: Any?): Boolean =
        when (other) {
            is Basis -> (this._x.x == other._x.x && this._x.y == other._x.y && this._x.z == other._x.z &&
                    this._y.x == other._y.x && this._y.y == other._y.y && this._y.z == other._y.z &&
                    this._z.x == other._z.x && this._z.y == other._z.y && this._z.z == other._z.z)
            else -> throw IllegalArgumentException()
        }

    override fun hashCode(): Int {
        var result = _x.hashCode()
        result = 31 * result + _y.hashCode()
        result = 31 * result + _z.hashCode()
        return result
    }


    fun set(xAxis: Vector3, yAxis: Vector3, zAxis: Vector3) {
        setAxis(0, xAxis)
        setAxis(1, yAxis)
        setAxis(2, zAxis)
    }

    /*
     * GDScript related members
     */
    constructor(xAxis: Vector3, yAxis: Vector3, zAxis: Vector3) : this() {
        set(xAxis, yAxis, zAxis)
    }

    //PROPERTIES
    /** Return a copy of the x Vector3
     * Warning: Writing x.x = 2 will only modify a copy, not the actual object.
     * To modify it, use x().
     * */
    var x
        get() = getAxis(0)
        set(value) {
            setAxis(0, value)
        }

    inline fun <T> x(block: Vector3.() -> T): T {
        return _x.block()
    }

    /** Return a copy of the y Vector3
     * Warning: Writing y.x = 2 will only modify a copy, not the actual object.
     * To modify it, use y().
     * */
    var y
        get() = getAxis(1)
        set(value) {
            setAxis(1, value)
        }


    inline fun <T> y(block: Vector3.() -> T): T {
        return _y.block()
    }

    /** Return a copy of the z Vector3
     * Warning: Writing z.x = 2 will only modify a copy, not the actual object.
     * To modify it, use z().
     * */
    var z
        get() = getAxis(2)
        set(value) {
            setAxis(2, value)
        }

    inline fun <T> z(block: Vector3.() -> T): T {
        return _z.block()
    }

    operator fun get(index: Int): Vector3 {
        return getAxis(index)
    }

    operator fun set(index: Int, value: Vector3) {
        return setAxis(index, value)
    }
}

operator fun Int.times(basis: Basis) = basis * this
operator fun Long.times(basis: Basis) = basis * this
operator fun Double.times(basis: Basis) = basis * this