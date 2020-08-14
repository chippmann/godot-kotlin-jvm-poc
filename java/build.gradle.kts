buildscript {
    repositories {
        jcenter()
    }
    dependencies {
        classpath("com.github.jengelman.gradle.plugins:shadow:5.0.0")
    }
}

plugins {
    kotlin("jvm") version "1.3.72"
}

apply(plugin = "com.github.johnrengelman.shadow")

group = "org.utopia-rise"
version = "0.0.1"

repositories {
    mavenCentral()
}

dependencies {
    implementation(kotlin("stdlib"))
}

val embeddable by configurations.creating {
    extendsFrom(configurations.implementation.get())
}

val shadowJar by tasks.getting(com.github.jengelman.gradle.plugins.shadow.tasks.ShadowJar::class) {
    configurations = listOf(embeddable)
    @Suppress("UnstableApiUsage")
    manifest {
        attributes["Implementation-Title"] = "Source Code"
        attributes["Implementation-Version"] = project.version
    }
    archiveVersion.set(project.version.toString())
    val classifier: String? = null //needed as we need to specify the type null represents. otherwise we get ambiguous overload exception during build
    archiveClassifier.set(classifier)
}