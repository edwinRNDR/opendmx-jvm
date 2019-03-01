# OPENDMX-JVM

A Java/JVM library for Enttec's OPENDMX USB adapter.

Currently only works on 64 bit JVMs and Windows.

Looks like this only builds with JDK 8.

# Usage

```groovy
repositories {
    maven {
        url = "https://dl.bintray.com/edwinrndr/opendmx-jvm"
    }
}

dependencies {
    compile "org.openrndr.opendmx:opendmx-jvm:0.0.1"
}
```

```kotlin
import org.openrndr.opendmx.Native

fun main(args: Array<String>) {
    val result = Native.connect(1)

    val ba = ByteArray(512) { 0.toByte() }
        Native.send(ba)

    while (true) {
        ba[3] = 200.toByte()
        Native.send(ba)
        Thread.sleep(100)
    }
}


```