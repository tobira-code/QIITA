import java.io.File

fun readBlock(pathname: String, size: Int, count: Int = 0, action: (ByteArray) -> Unit) {
    val input = File(pathname).inputStream()
    val buf = ByteArray(size)

    if (count == 0) {
        while (input.read(buf) == buf.size) {
            action(buf)
        }
    } else {
        var c = 0
        while (input.read(buf) == buf.size && c < count) {
            action(buf)
            c += 1
        }
    }
}

fun printByteArrayAsHexString(buf: ByteArray) {
    repeat(buf.size) {
        print(String.format("%02x ", buf[it]))
    }
    println()
}

fun byte2BinaryString(b: Int): String {
    var s = ""
    repeat(8) {
        s += String.format("%d", (b shr (7 - it)) and 1)
    }
    return s
}

fun printByteArrayAsBinaryString(buf: ByteArray) {
    repeat(buf.size) {
        print(String.format("%s ", byte2BinaryString(buf[it].toInt())))
    }
    println()
}

fun main() {
    readBlock("test.ts", 188, 1) {
        printByteArrayAsHexString(it)
        printByteArrayAsBinaryString(it)
    }
}import java.io.File

fun readBlock(pathname: String, size: Int, count: Int = 0, action: (ByteArray) -> Unit) {
    val input = File(pathname).inputStream()
    val buf = ByteArray(size)

    if (count == 0) {
        while (input.read(buf) == buf.size) {
            action(buf)
        }
    } else {
        var c = 0
        while (input.read(buf) == buf.size && c < count) {
            action(buf)
            c += 1
        }
    }
}

fun printByteArrayAsHexString(buf: ByteArray) {
    repeat(buf.size) {
        print(String.format("%02x ", buf[it]))
    }
    println()
}

fun byte2BinaryString(b: Int): String {
    var s = ""
    repeat(8) {
        s += String.format("%d", (b shr (7 - it)) and 1)
    }
    return s
}

fun printByteArrayAsBinaryString(buf: ByteArray) {
    repeat(buf.size) {
        print(String.format("%s ", byte2BinaryString(buf[it].toInt())))
    }
    println()
}

fun main() {
    readBlock("test.ts", 188, 1) {
        printByteArrayAsHexString(it)
        printByteArrayAsBinaryString(it)
    }
}
