package godot.registration

import godot.core.Object

@DslMarker
@Target(AnnotationTarget.CLASS)
@Retention(AnnotationRetention.SOURCE)
annotation class ClassBuilderDSL

@ClassBuilderDSL
class ClassBuilder<T : Object> internal constructor(val classHandle: ClassHandle<T>) {

}