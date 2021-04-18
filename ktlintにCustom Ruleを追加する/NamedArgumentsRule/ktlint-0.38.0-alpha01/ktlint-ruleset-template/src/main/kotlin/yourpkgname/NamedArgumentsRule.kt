package yourpkgname

import com.pinterest.ktlint.core.Rule
import com.pinterest.ktlint.core.ast.ElementType.CALL_EXPRESSION
import com.pinterest.ktlint.core.ast.ElementType.VALUE_ARGUMENT
import com.pinterest.ktlint.core.ast.ElementType.VALUE_ARGUMENT_LIST
import com.pinterest.ktlint.core.ast.ElementType.VALUE_ARGUMENT_NAME
import com.pinterest.ktlint.core.ast.children
import org.jetbrains.kotlin.com.intellij.lang.ASTNode

class NamedArgumentsRule : Rule("named-arguments") {

    override fun visit(
        node: ASTNode,
        autoCorrect: Boolean,
        emit: (offset: Int, errorMessage: String, canBeAutoCorrected: Boolean) -> Unit
    ) {
        if (node.elementType == CALL_EXPRESSION) {
            val valueArgumentList = node.children().elementAt(1)
            if (valueArgumentList.elementType == VALUE_ARGUMENT_LIST) {
                valueArgumentList.children().forEach {
                    if (it.elementType == VALUE_ARGUMENT) {
                        if (!hasNamedArgument(it)) {
                            emit(it.startOffset, "found a function with no named arguments", false)
                        }
                    }
                }
            }
        }
    }

    private fun hasNamedArgument(node: ASTNode): Boolean {
        node.children().forEach {
            if (it.elementType == VALUE_ARGUMENT_NAME) {
                return true
            }
        }
        return false
    }
}
