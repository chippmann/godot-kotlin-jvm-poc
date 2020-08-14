package godot.core

class Transform(
    p_basis: Basis,
    p_origin: Vector3
) {

    @PublishedApi
    internal var _basis = Basis(p_basis)
    @PublishedApi
    internal var _origin = Vector3(p_origin)


    //PROPERTIES
    /** Return a copy of the basis Basis.
     * Warning: Writing basis.x = 2 will only modify a copy, not the actual object.
     * To modify it, use basis().
     * */
    var basis
        get() = Basis(_basis)
        set(value) {
            _basis = Basis(value)
        }

    inline fun <T> basis(block: Basis.() -> T): T {
        return _basis.block()
    }

    /** Return a copy of the origin Vector3
     * Warning: Writing origin.x = 2 will only modify a copy, not the actual object.
     * To modify it, use origin().
     * */
    var origin
        get() = Vector3(_origin)
        set(value) {
            _origin = Vector3(value)
        }

    inline fun <T> origin(block: Vector3.() -> T): T {
        return _origin.block()
    }



    //CONSTRUCTOR
    constructor() :
            this(Basis(), Vector3(0.0, 0.0, 0.0))

    constructor(other: Transform) :
            this(other._basis, other._origin)

    constructor(
        xx: Double,
        xy: Double,
        xz: Double,
        yx: Double,
        yy: Double,
        yz: Double,
        zx: Double,
        zy: Double,
        zz: Double,
        tx: Double,
        ty: Double,
        tz: Double
    ) :
            this(Basis(xx, xy, xz, yx, yy, yz, zx, zy, zz), Vector3(tx, ty, tz))







    /**
     * Returns the inverse of the transform, under the assumption that the transformation is composed of rotation and translation (no scaling, use affine_inverse for transforms with scaling).
     */
    fun inverse(): Transform {
        val ret = Transform(this._basis, this._origin)
        ret.invert()
        return ret
    }

    internal fun invert() {
        _basis.transpose()
        _origin = _basis.xform(-_origin)
    }






    /**
     * Rotates the transform around the given axis by the given angle (in radians), using matrix multiplication. The axis must be a normalized vector.
     */
    fun rotated(axis: Vector3, phi: Double): Transform {
        return Transform(Basis(axis, phi), Vector3()) * this
    }

    internal fun rotate(axis: Vector3, phi: Double) {
        val t = rotated(axis, phi)
        this._basis = t._basis
        this._origin = t._origin
    }

    /**
     * Scales basis and origin of the transform by the given scale factor, using matrix multiplication.
     */
    fun scaled(scale: Vector3): Transform {
        val t = Transform(this._basis, this._origin)
        t.scale(scale)
        return t
    }

    fun scale(scale: Vector3) {
        _basis.scale(scale)
        _origin *= scale
    }

    /**
     * Translates the transform by the given offset, relative to the transform’s basis vectors.
     * Unlike rotated and scaled, this does not use matrix multiplication.
     */
    fun translated(translation: Vector3): Transform {
        val t = Transform(this._basis, this._origin)
        t.translate(translation)
        return t
    }

    fun translate(translation: Vector3) {
        _origin.x += _basis._x.dot(translation)
        _origin.y += _basis._y.dot(translation)
        _origin.z += _basis._z.dot(translation)
    }

    /**
     * Transforms the given Vector3 by this transform.
     */
    fun xform(vector: Vector3): Vector3 =
        Vector3(
            _basis._x.dot(vector) + _origin.x,
            _basis._y.dot(vector) + _origin.y,
            _basis._z.dot(vector) + _origin.z
        )



    /**
     * Inverse-transforms the given Vector3 by this transform.
     */
    fun xformInv(vector: Vector3): Vector3 {
        val v = vector - _origin
        return Vector3(
            (_basis._x.x * v.x) + (_basis._y.x * v.y) + (_basis._z.x * v.z),
            (_basis._x.y * v.x) + (_basis._y.y * v.y) + (_basis._z.y * v.z),
            (_basis._x.z * v.x) + (_basis._y.z * v.y) + (_basis._z.z * v.z)
        )
    }





    operator fun times(transform: Transform): Transform {
        val t = this
        t._origin = xform(transform._origin)
        t._basis *= transform._basis
        return t
    }

    override fun equals(other: Any?): Boolean {
        return when (other) {
            is Transform -> _basis == other._basis && _origin == other._origin
            else -> false
        }
    }

    override fun toString(): String {
        return "$_basis - $_origin"
    }

    override fun hashCode(): Int {
        var result = _basis.hashCode()
        result = 31 * result + _origin.hashCode()
        return result
    }

    /*
     * GDScript related members
     */
    constructor(x: Vector3, y: Vector3, z: Vector3, origin: Vector3) :
            this(Basis(x, y, z), origin)
}