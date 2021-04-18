---
title: ktlintにCustom Ruleを追加する
tags: Kotlin ktlint AndroidStudio Android
author: tobira-code
slide: false
---
# はじめに
ktlintの使い方、およびCustom Ruleを追加する方法を説明します。
ktlintはKotlin Codeの形式的なチェックをおこないます。Ruleの例は[2]を参照ください。
可能な範囲で自動Formatできます。

基本的な使い方を説明した後に、Custom Ruleを追加する方法を説明します。

# 環境
Android Studioを使います。Versionの詳細は以下の通りです。

```
Android Studio 4.0
Build #AI-193.6911.18.40.6514223, built on May 21, 2020
Runtime version: 1.8.0_242-release-1644-b01 amd64
VM: OpenJDK 64-Bit Server VM by JetBrains s.r.o
Windows 10 10.0
```

# ktlintを使う
ktlintの基本的な使い方を説明します。
手順は次の通りです。

- Android Studioを起動します。"Welcome to Android Studio"の画面が表示されます。(*1)
- "Start a new Android Studio project"を選択します。
- "No Activity"を選択します。Nextを押下します。
- ここでは次のように設定しました。

(*1)表示されない場合は[File]-[Close Project]をおこないます。

```
Name -> My Application
Package name -> com.e.myapplication
Language -> Kotlin
Minimum SDK -> API 30: Android 10.0+(R)
```

- Finishを押下します。
- app\build.gradleの最後に以下を追加します。([1] without a pluginを引用します)
- com.pinterest:ktlint:0.41.0 -> com.pinterest:ktlint:0.38.0-alpha01 に置き換えます。

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

- 画面右上の"Sync now"を実行します。
- [View]-[Tool Windows]-[Gradle]を選択します。
- Gradle [My Application]-[Tasks]-[verification]-[ktlint]を押下します。

実行後に次のエラーが表示されます。

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

次の手順で自動Formatします。

- [View]-[Tool Windows]-[Gradle]を選択します。
- Gradle [My Application]-[Tasks]-[formatting]-[ktlintFormat]を押下します。

自動Formatによりエラー数が減りました。

```
> Task :app:ktlint FAILED
C:\Users\[User]\AndroidStudioProjects\MyApplication\app\src\androidTest\java\com\e\myapplication\ExampleInstrumentedTest.kt:5:1: Wildcard import (cannot be auto-corrected)
C:\Users\[User]\AndroidStudioProjects\MyApplication\app\src\test\java\com\e\myapplication\ExampleUnitTest.kt:3:1: Wildcard import (cannot be auto-corrected)
1 actionable task: 1 executed
```

# Ruleを作る
ktlintではユーザが独自にRuleを作ることができます。
ktlintのgitに含まれるktlint-ruleset-templateを例に説明します。

0.38.0-alpha01を使いました。以下を取得して、展開してください。
https://github.com/pinterest/ktlint/archive/refs/tags/0.38.0-alpha01.zip

## ktlint-ruleset-template
ktlint-ruleset-templateはvarが使用できないRuleを定義しています。

ktlint-ruleset-templateの処理内容を説明します。

ktlint-ruleset-template\src\main\kotlin\yourpkgname
NoVarRule.ktを説明します。

Rule classを継承したNoVarRuleを実装します。
visitはAbstract syntax tree(AST)のnodeを訪れます。
visitを実装することでRuleに適合しているかどうかをチェックできます。
elementTypeがVAR_KEYWORDであればemitを実行します。
emitによりエラー発生をktlintに返します。

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
CustomRuleSetProvider.ktを説明します。

NoVarRuleをktlintに登録するためにRuleSetProviderを実装します。
getをoverrideします。getはRuleSetを返します。RuleSetにNoVerRuleを登録します。

```kotlin:CustomRuleSetProvider.kt
package yourpkgname

import com.pinterest.ktlint.core.RuleSet
import com.pinterest.ktlint.core.RuleSetProvider

class CustomRuleSetProvider : RuleSetProvider {

    override fun get(): RuleSet = RuleSet("custom", NoVarRule())
}
```

なお、RuleSetには次のように複数のRuleを設定できます。
例えばNoValRule()を追加したい場合は次のようにします。

```
    override fun get(): RuleSet = RuleSet("custom", NoVarRule(), NoValRule())
```

CustomRuleSetProviderを次のファイルの登録します。

ktlint-ruleset-template\src\main\resources\META-INF\services\com.pinterest.ktlint.core.RuleSetProvider

```:com.pinterest.ktlint.core.RuleSetProvider
yourpkgname.CustomRuleSetProvider
```

## jarの作り方

まず、Ruleのjarを作ります。
次に、jarをktlintを実行するProjectに取り込み、実行します。

jarの作り方を説明します。

- [1]から0.38.0-alpha01を取得します。展開します。
https://github.com/pinterest/ktlint/archive/refs/tags/0.38.0-alpha01.zip

- Android Studioを起動します。
- "Import project(Gradle, EclipseADT, etc.)"により、展開したディレクトリをImportします。
※MAVEN_REPOSITORYの定義がないことによるエラーが表示されます。
  Ruleを作ることには影響がないため無視します。

- [View]-[Tool Windows]-[Gradle]を選択します。
- Gradle [ktlint]-[ktlint-ruleset-template]-[Tasks]-[build]-[jar]を押下します。
- ktlint-ruleset-template.jarができます。
ktlint-ruleset-template\build\libsに配置されます。

## jarの使い方
jarの使い方は次の通りです。

- [ktlintを使う](#ktlintを使う) で作ったProjectを開きます。
- app\libsにktlint-ruleset-template.jarをコピーします。
- app\build.graldeのtask ktlintを以下のように変更します。

```:app\build.gradle
-    args "src/**/*.kt"
+    args '--debug', 'src/**/*.kt', '-R', 'libs/ktlint-ruleset-template.jar'
```

- app\src\test\java\com\e\myapplication\ExampleUnitTest.ktを以下のように変更します。

```kotlin:
        assertEquals(4, 2 + 2)
+        var v = "val"
```

- 画面右上の"Sync now"を実行します。
- [View]-[Tool Windows]-[Gradle]を選択します。
- Gradle [My Application]-[Tasks]-[verification]-[ktlint]を押下します。

次のエラーが表示されていればRuleが適用できています。

```
C:\Users\[User]\AndroidStudioProjects\MyApplication\app\src\test\java\com\e\myapplication\ExampleUnitTest.kt:15:9: Unexpected var, use val instead (cannot be auto-corrected)
```

## 自動Formatする
自動でvarをvalに変換します。

ktlint-ruleset-template\src\main\kotlin\yourpkgname
NoVarRule.ktを変更します。

ktlintFormatを実行するとcanBeAutoCorrected=trueが指定されてvisitが呼ばれます。
trueの場合、replaceWithTextでvarをvalに置き換えます。

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

- jarを作成します。
- [ktlintを使う](#ktlintを使う) で作ったProjectを開きます。
- app\libsにktlint-ruleset-template.jarをコピーします。
- app\build.graldeのtask ktlintFormatを以下のように変更します。

```:app\build.gradle
-   args "-F", "src/**/*.kt"
+   args '-F', '--debug', 'src/**/*.kt', '-R', 'libs/ktlint-ruleset-template.jar'
```

- [View]-[Tool Windows]-[Gradle]を選択します。
- Gradle [My Application]-[Tasks]-[formatting]-[ktlintFormat]を押下します。
- Gradle [My Application]-[Tasks]-[verification]-[ktlint]を押下します。

次のエラーが表示されなくなります。

```
C:\Users\[User]\AndroidStudioProjects\MyApplication\app\src\test\java\com\e\myapplication\ExampleUnitTest.kt:15:9: Unexpected var, use val instead
```

## Abstract syntax tree(AST)を確認する
Ruleを作る際、ASTを確認できると便利です。
ktlin の引数 "--print-ast" を設定することで出力されます。

- app\build.graldeのtask ktlintを以下のように変更します。

```:app\build.gradle
-    args '--debug', 'src/**/*.kt', '-R', 'libs/ktlint-ruleset-template.jar'
+    args "--print-ast", "src/**/*.kt"
```

ktlintを実行します。出力の一部を抜粋します。

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
