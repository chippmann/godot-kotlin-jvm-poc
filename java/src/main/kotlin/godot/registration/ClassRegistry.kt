package godot.registration

import godot.core.Object

class ClassRegistry(private val nativescriptHandle: Int) {
    fun <T : Object> registerClass(
        name: String,
        parent: String,
        factory: () -> T,
        isTool: Boolean,
        builder: ClassBuilder<T>.() -> Unit
    ) {
        val handle = ClassHandle(nativescriptHandle, name, parent, factory, isTool)
        handle.init()
//        TypeManager.registerUserType(nativescriptHandle, name, factory)
        builder(ClassBuilder(handle))
    }
}