---
title: ktlint��Custom Rule��ǉ�����
tags: Kotlin ktlint AndroidStudio Android
author: tobira-code
slide: false
---
# �͂��߂�
ktlint�̎g�����A�����Custom Rule��ǉ�������@��������܂��B
ktlint��Kotlin Code�̌`���I�ȃ`�F�b�N�������Ȃ��܂��BRule�̗��[2]���Q�Ƃ��������B
�\�Ȕ͈͂Ŏ���Format�ł��܂��B

��{�I�Ȏg���������������ɁACustom Rule��ǉ�������@��������܂��B

# ��
Android Studio���g���܂��BVersion�̏ڍׂ͈ȉ��̒ʂ�ł��B

```
Android Studio 4.0
Build #AI-193.6911.18.40.6514223, built on May 21, 2020
Runtime version: 1.8.0_242-release-1644-b01 amd64
VM: OpenJDK 64-Bit Server VM by JetBrains s.r.o
Windows 10 10.0
```

# ktlint���g��
ktlint�̊�{�I�Ȏg������������܂��B
�菇�͎��̒ʂ�ł��B

- Android Studio���N�����܂��B"Welcome to Android Studio"�̉�ʂ��\������܂��B(*1)
- "Start a new Android Studio project"��I�����܂��B
- "No Activity"��I�����܂��BNext���������܂��B
- �����ł͎��̂悤�ɐݒ肵�܂����B

(*1)�\������Ȃ��ꍇ��[File]-[Close Project]�������Ȃ��܂��B

```
Name -> My Application
Package name -> com.e.myapplication
Language -> Kotlin
Minimum SDK -> API 30: Android 10.0+(R)
```

- Finish���������܂��B
- app\build.gradle�̍Ō�Ɉȉ���ǉ����܂��B([1] without a plugin�����p���܂�)
- com.pinterest:ktlint:0.41.0 -> com.pinterest:ktlint:0.38.0-alpha01 �ɒu�������܂��B

```build.gradle
configurations {
    ktlint
}

dependencies {
    ktlint "com.pinterest:ktlint:0.38.0-alpha01"
    // additional 3rd party ruleset(s) can be specified here
    // just add them to the classpath (e.g. ktlint 'groupId:artifactId:version') and
    // ktlint will pick them up
}

task ktlint(type: JavaExec, group: "verification") {
    description = "Check Kotlin code style."
    classpath = configurations.ktlint
    main = "com.pinterest.ktlint.Main"
    args "src/**/*.kt"
    // to generate report in checkstyle format prepend following args:
    // "--reporter=plain", "--reporter=checkstyle,output=${buildDir}/ktlint.xml"
    // to add a baseline to check against prepend following args:
    // "--baseline=ktlint-baseline.xml"
    // see https://github.com/pinterest/ktlint#usage for more
}
check.dependsOn ktlint

task ktlintFormat(type: JavaExec, group: "formatting") {
    description = "Fix Kotlin code style deviations."
    classpath = configurations.ktlint
    main = "com.pinterest.ktlint.Main"
    args "-F", "src/**/*.kt"
}
```

- ��ʉE���"Sync now"�����s���܂��B
- [View]-[Tool Windows]-[Gradle]��I�����܂��B
- Gradle [My Application]-[Tasks]-[verification]-[ktlint]���������܂��B

���s��Ɏ��̃G���[���\������܂��B

```
> Task :app:ktlint FAILED
C:\Users\[User]\AndroidStudioProjects\MyApplication\app\src\androidTest\java\com\e\myapplication\ExampleInstrumentedTest.kt:1:1: File must end with a newline (\n)
C:\Users\[User]\AndroidStudioProjects\MyApplication\app\src\androidTest\java\com\e\myapplication\ExampleInstrumentedTest.kt:3:1: Imports must be ordered in lexicographic order without any empty lines in-between with "java", "javax", "kotlin" and aliases in the end
C:\Users\[User]\AndroidStudioProjects\MyApplication\app\src\androidTest\java\com\e\myapplication\ExampleInstrumentedTest.kt:9:1: Wildcard import (cannot be auto-corrected)
C:\Users\[User]\AndroidStudioProjects\MyApplication\app\src\test\java\com\e\myapplication\ExampleUnitTest.kt:1:1: File must end with a newline (\n)
C:\Users\[User]\AndroidStudioProjects\MyApplication\app\src\test\java\com\e\myapplication\ExampleUnitTest.kt:3:1: Imports must be ordered in lexicographic order without any empty lines in-between with "java", "javax", "kotlin" and aliases in the end
C:\Users\[User]\AndroidStudioProjects\MyApplication\app\src\test\java\com\e\myapplication\ExampleUnitTest.kt:5:1: Wildcard import (cannot be auto-corrected)
1 actionable task: 1 executed
```

���̎菇�Ŏ���Format���܂��B

- [View]-[Tool Windows]-[Gradle]��I�����܂��B
- Gradle [My Application]-[Tasks]-[formatting]-[ktlintFormat]���������܂��B

����Format�ɂ��G���[��������܂����B

```
> Task :app:ktlint FAILED
C:\Users\[User]\AndroidStudioProjects\MyApplication\app\src\androidTest\java\com\e\myapplication\ExampleInstrumentedTest.kt:5:1: Wildcard import (cannot be auto-corrected)
C:\Users\[User]\AndroidStudioProjects\MyApplication\app\src\test\java\com\e\myapplication\ExampleUnitTest.kt:3:1: Wildcard import (cannot be auto-corrected)
1 actionable task: 1 executed
```

# Rule�����
ktlint�ł̓��[�U���Ǝ���Rule����邱�Ƃ��ł��܂��B
ktlint��git�Ɋ܂܂��ktlint-ruleset-template���ɐ������܂��B

0.38.0-alpha01���g���܂����B�ȉ����擾���āA�W�J���Ă��������B
https://github.com/pinterest/ktlint/archive/refs/tags/0.38.0-alpha01.zip

## ktlint-ruleset-template
ktlint-ruleset-template��var���g�p�ł��Ȃ�Rule���`���Ă��܂��B

ktlint-ruleset-template�̏������e��������܂��B

ktlint-ruleset-template\src\main\kotlin\yourpkgname
NoVarRule.kt��������܂��B

Rule class���p������NoVarRule���������܂��B
visit��Abstract syntax tree(AST)��node��K��܂��B
visit���������邱�Ƃ�Rule�ɓK�����Ă��邩�ǂ������`�F�b�N�ł��܂��B
elementType��VAR_KEYWORD�ł����emit�����s���܂��B
emit�ɂ��G���[������ktlint�ɕԂ��܂��B

```kotlin:NoVarRule.kt
package yourpkgname

import com.pinterest.ktlint.core.Rule
import com.pinterest.ktlint.core.ast.ElementType.VAR_KEYWORD
import org.jetbrains.kotlin.com.intellij.lang.ASTNode

class NoVarRule : Rule("no-var") {

    override fun visit(
        node: ASTNode,
        autoCorrect: Boolean,
        emit: (offset: Int, errorMessage: String, canBeAutoCorrected: Boolean) -> Unit
    ) {
        if (node.elementType == VAR_KEYWORD) {
            emit(node.startOffset, "Unexpected var, use val instead", false)
        }
    }
}
```

ktlint-ruleset-template\src\main\kotlin\yourpkgname
CustomRuleSetProvider.kt��������܂��B

NoVarRule��ktlint�ɓo�^���邽�߂�RuleSetProvider���������܂��B
get��override���܂��Bget��RuleSet��Ԃ��܂��BRuleSet��NoVerRule��o�^���܂��B

```kotlin:CustomRuleSetProvider.kt
package yourpkgname

import com.pinterest.ktlint.core.RuleSet
import com.pinterest.ktlint.core.RuleSetProvider

class CustomRuleSetProvider : RuleSetProvider {

    override fun get(): RuleSet = RuleSet("custom", NoVarRule())
}
```

�Ȃ��ARuleSet�ɂ͎��̂悤�ɕ�����Rule��ݒ�ł��܂��B
�Ⴆ��NoValRule()��ǉ��������ꍇ�͎��̂悤�ɂ��܂��B

```
    override fun get(): RuleSet = RuleSet("custom", NoVarRule(), NoValRule())
```

CustomRuleSetProvider�����̃t�@�C���̓o�^���܂��B

ktlint-ruleset-template\src\main\resources\META-INF\services\com.pinterest.ktlint.core.RuleSetProvider

```:com.pinterest.ktlint.core.RuleSetProvider
yourpkgname.CustomRuleSetProvider
```

## jar�̍���

�܂��ARule��jar�����܂��B
���ɁAjar��ktlint�����s����Project�Ɏ�荞�݁A���s���܂��B

jar�̍�����������܂��B

- [1]����0.38.0-alpha01���擾���܂��B�W�J���܂��B
https://github.com/pinterest/ktlint/archive/refs/tags/0.38.0-alpha01.zip

- Android Studio���N�����܂��B
- "Import project(Gradle, EclipseADT, etc.)"�ɂ��A�W�J�����f�B���N�g����Import���܂��B
��MAVEN_REPOSITORY�̒�`���Ȃ����Ƃɂ��G���[���\������܂��B
  Rule����邱�Ƃɂ͉e�����Ȃ����ߖ������܂��B

- [View]-[Tool Windows]-[Gradle]��I�����܂��B
- Gradle [ktlint]-[ktlint-ruleset-template]-[Tasks]-[build]-[jar]���������܂��B
- ktlint-ruleset-template.jar���ł��܂��B
ktlint-ruleset-template\build\libs�ɔz�u����܂��B

## jar�̎g����
jar�̎g�����͎��̒ʂ�ł��B

- [ktlint���g��](#ktlint���g��) �ō����Project���J���܂��B
- app\libs��ktlint-ruleset-template.jar���R�s�[���܂��B
- app\build.gralde��task ktlint���ȉ��̂悤�ɕύX���܂��B

```:app\build.gradle
-    args "src/**/*.kt"
+    args '--debug', 'src/**/*.kt', '-R', 'libs/ktlint-ruleset-template.jar'
```

- app\src\test\java\com\e\myapplication\ExampleUnitTest.kt���ȉ��̂悤�ɕύX���܂��B

```kotlin:
        assertEquals(4, 2 + 2)
+        var v = "val"
```

- ��ʉE���"Sync now"�����s���܂��B
- [View]-[Tool Windows]-[Gradle]��I�����܂��B
- Gradle [My Application]-[Tasks]-[verification]-[ktlint]���������܂��B

���̃G���[���\������Ă����Rule���K�p�ł��Ă��܂��B

```
C:\Users\[User]\AndroidStudioProjects\MyApplication\app\src\test\java\com\e\myapplication\ExampleUnitTest.kt:15:9: Unexpected var, use val instead (cannot be auto-corrected)
```

## ����Format����
������var��val�ɕϊ����܂��B

ktlint-ruleset-template\src\main\kotlin\yourpkgname
NoVarRule.kt��ύX���܂��B

ktlintFormat�����s�����canBeAutoCorrected=true���w�肳���visit���Ă΂�܂��B
true�̏ꍇ�AreplaceWithText��var��val�ɒu�������܂��B

```kotlin:NoVarRule.kt
class NoVarRule : Rule("no-var") {

    override fun visit(
        node: ASTNode,
        autoCorrect: Boolean,
        emit: (offset: Int, errorMessage: String, canBeAutoCorrected: Boolean) -> Unit
    ) {
        if (node.elementType == VAR_KEYWORD) {
-            emit(node.startOffset, "Unexpected var, use val instead", false)
+            emit(node.startOffset, "Unexpected var, use val instead", true)
+            if (autoCorrect) {
+                (node.psi as LeafPsiElement).replaceWithText("val")
+            }
        }
    }
}
```

- jar���쐬���܂��B
- [ktlint���g��](#ktlint���g��) �ō����Project���J���܂��B
- app\libs��ktlint-ruleset-template.jar���R�s�[���܂��B
- app\build.gralde��task ktlintFormat���ȉ��̂悤�ɕύX���܂��B

```:app\build.gradle
-   args "-F", "src/**/*.kt"
+   args '-F', '--debug', 'src/**/*.kt', '-R', 'libs/ktlint-ruleset-template.jar'
```

- [View]-[Tool Windows]-[Gradle]��I�����܂��B
- Gradle [My Application]-[Tasks]-[formatting]-[ktlintFormat]���������܂��B
- Gradle [My Application]-[Tasks]-[verification]-[ktlint]���������܂��B

���̃G���[���\������Ȃ��Ȃ�܂��B

```
C:\Users\[User]\AndroidStudioProjects\MyApplication\app\src\test\java\com\e\myapplication\ExampleUnitTest.kt:15:9: Unexpected var, use val instead
```

## Abstract syntax tree(AST)���m�F����
Rule�����ہAAST���m�F�ł���ƕ֗��ł��B
ktlin �̈��� "--print-ast" ��ݒ肷�邱�Ƃŏo�͂���܂��B

- app\build.gralde��task ktlint���ȉ��̂悤�ɕύX���܂��B

```:app\build.gradle
-    args '--debug', 'src/**/*.kt', '-R', 'libs/ktlint-ruleset-template.jar'
+    args "--print-ast", "src/**/*.kt"
```

ktlint�����s���܂��B�o�͂̈ꕔ�𔲐����܂��B

```
> Task :app:ktlint
 1: ~.psi.KtFile (FILE)
 1:   ~.psi.KtPackageDirective (PACKAGE_DIRECTIVE)
 1:     ~.c.i.p.impl.source.tree.LeafPsiElement (PACKAGE_KEYWORD) "package"
 1:     ~.c.i.p.impl.source.tree.PsiWhiteSpaceImpl (WHITE_SPACE) " "
 1:     ~.psi.KtDotQualifiedExpression (DOT_QUALIFIED_EXPRESSION)
 1:       ~.psi.KtDotQualifiedExpression (DOT_QUALIFIED_EXPRESSION)
...
```

# References
- [1] https://github.com/pinterest/ktlint
- [2] https://ktlint.github.io
