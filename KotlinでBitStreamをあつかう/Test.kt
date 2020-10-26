import kotlin.test.assertEquals

class BitStreamReader(private val byteArray: ByteArray, private val startBitOffset: Int = 0) {
    private var currentBitOffset: Int = 0

    fun reset() {
        currentBitOffset = 0
    }

    fun readBits(bitLength: Int, bitOffset: Int = 0): Long {
        var result: Long = 0
        var bitOffset = startBitOffset + currentBitOffset + bitOffset
        repeat(bitLength) {
            result = result shl 1
            result = result or read1bit(it + bitOffset)
        }
        return result
    }

    fun advanceCurrentBitOffset(bitLength: Int) {
        currentBitOffset += bitLength
    }

    fun readBitsAndAdvanceCurrentBitOffset(bitLength: Int): Long {
        val result = readBits(bitLength)
        advanceCurrentBitOffset(bitLength)
        return result
    }

    private fun read1bit(offset: Int): Long {
        val byteOffset = offset.ushr(3)
        val bitOffset = 7 - offset % 8
        val result: Long = byteArray[byteOffset].toLong()
        return result.ushr(bitOffset).and(1)
    }
}

fun main() {
    val buf = byteArrayOf(0b1010_1100.toByte(), 0b1110_0001.toByte())
    val bsr = BitStreamReader(buf)

    assertEquals(0b1, bsr.readBits(1))
    assertEquals(0b10, bsr.readBits(2))
    assertEquals(0b1010_1100_1110_000, bsr.readBits(15))
    assertEquals(0b1010_1100_1110_0001, bsr.readBits(16))

    bsr.advanceCurrentBitOffset(1)
    assertEquals(0b0, bsr.readBits(1))
    assertEquals(0b010_1100_1110_000, bsr.readBits(14))
    assertEquals(0b010_1100_1110_0001, bsr.readBits(15))

    bsr.reset()
    assertEquals(0b1, bsr.readBits(1))
    assertEquals(0b10, bsr.readBits(2))
    assertEquals(0b1010_1100_1110_000, bsr.readBits(15))
    assertEquals(0b1010_1100_1110_0001, bsr.readBits(16))

    assertEquals(0b1, bsr.readBitsAndAdvanceCurrentBitOffset(1))
    assertEquals(0b01, bsr.readBitsAndAdvanceCurrentBitOffset(2))
    assertEquals(0b011, bsr.readBitsAndAdvanceCurrentBitOffset(3))
    assertEquals(0b0011, bsr.readBitsAndAdvanceCurrentBitOffset(4))

    bsr.reset()
    assertEquals(0b1, bsr.readBits(1))
    assertEquals(0b10, bsr.readBits(2))
    assertEquals(0b1010_1100_1110_000, bsr.readBits(15))
    assertEquals(0b1010_1100_1110_0001, bsr.readBits(16))
}
