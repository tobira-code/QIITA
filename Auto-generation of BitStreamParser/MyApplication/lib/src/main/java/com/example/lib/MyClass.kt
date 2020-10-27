import org.antlr.v4.runtime.*
import org.antlr.v4.runtime.tree.*
import java.io.File

fun String.snakeToLowerCamel(): String {
    return Regex("_[a-zA-Z]").replace(this.toLowerCase()) {
        it.value.replace("_", "")
            .toUpperCase()
    }
}

data class ElementInfo(val name: String, val bitLength: Int)

class CustomBitstreamListener : BitstreamListener {
    var bitOffset = 0
    var syntaxName = ""
    var elementInfoList = mutableListOf<ElementInfo>()

    override fun enterSyntax(ctx: BitstreamParser.SyntaxContext?) {
        bitOffset = 0 // reset
        elementInfoList.clear() // reset
        syntaxName = ctx!!.Identifier().toString().snakeToLowerCamel().capitalize()
        val str =
            """
            class $syntaxName(private val bitStreamReader: BitStreamReader) {

            """.trimIndent()
        File("$syntaxName.kt").writeText(str)
    }

    override fun exitSyntax(ctx: BitstreamParser.SyntaxContext?) {
        val file = File("$syntaxName.kt")
        file.appendText("    fun print() {\n")
        elementInfoList.forEach() {
            file.appendText("        print(\"${it.name} = \" + String.format(\"0x%04X\", ${it.name.snakeToLowerCamel()}()) + \" \")\n")
        }
        file.appendText("    }\n")
        file.appendText("} // $syntaxName \n")
    }

    override fun enterElement(ctx: BitstreamParser.ElementContext?) {
        val bitLength = ctx!!.DecimalConstant().toString().toInt()
        val name = ctx!!.Identifier().toString()
        val str =
            """    fun ${name.snakeToLowerCamel()}(): Int {
        return bitStreamReader.readBits($bitLength, $bitOffset).toInt()
    }

"""
        File("$syntaxName.kt").appendText(str)
        bitOffset += bitLength
        elementInfoList.add(ElementInfo(name, bitLength))
    }

    override fun exitElement(ctx: BitstreamParser.ElementContext?) {
    }

    override fun enterCompilationUnit(ctx: BitstreamParser.CompilationUnitContext?) {
    }

    override fun enterSyntaxList(ctx: BitstreamParser.SyntaxListContext?) {
    }

    override fun exitElemntList(ctx: BitstreamParser.ElemntListContext?) {
    }

    override fun enterEveryRule(ctx: ParserRuleContext?) {
    }

    override fun exitSyntaxList(ctx: BitstreamParser.SyntaxListContext?) {
    }

    override fun enterElemntList(ctx: BitstreamParser.ElemntListContext?) {
    }

    override fun exitEveryRule(ctx: ParserRuleContext?) {
    }

    override fun visitErrorNode(node: ErrorNode?) {
    }

    override fun exitCompilationUnit(ctx: BitstreamParser.CompilationUnitContext?) {
    }

    override fun visitTerminal(node: TerminalNode?) {
    }
}

fun main(args: Array<String>) {
    val string = if (args.size == 1) {
        File(args[0]).readText()
    } else {
        println("error args.size != 1")
        return
    }

    val text = CharStreams.fromString(string)
    val parser = BitstreamParser(CommonTokenStream(BitstreamLexer(text)))
    ParseTreeWalker.DEFAULT.walk(CustomBitstreamListener(), parser.compilationUnit())
}