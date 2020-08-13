package godot.registration

import godot.core.Object
import java.util.concurrent.Callable

class ClassHandle<T: Object>(
    private val nativescriptHandle: Int,
    private val className: String,
    private val parentClassName: String,
    private val factory: () -> T,
    private val isTool: Boolean
) {
    external fun wrap(instancePtr: Int, factory: Callable<T>)
    external fun init()
    external fun registerFunction(methodName: String, methodPtr: Int, rpcModeOrdinal: Int)
}