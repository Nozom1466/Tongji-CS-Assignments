# Compiler Design - Lexical Analyzer & Syntactic analyzer



1. 安装 `graphviz`

在文件夹  `./graphviz`  中点击 `windows_10_cmake_Release_graphviz-install-9.0.0-win64.exe` 安装 `graphviz`





2. 配置环境变量

在环境变量的系统变量中添加条目：

```cmd
..\graphviz\install_files\bin
```

> 路径为刚安装的 `graphviz` 文件夹中 `bin` 目录





3. 打开终端检查安装是否成功

输入 

```cmd
dot -V
```

显示 `graphviz` 版本号，如：

```cmd
dot - graphviz version 2.38.0 (20140413.2041)

```





4. 运行程序

进入根文件夹中 `source_code` 文件夹，终端输入

```cmd
python display.py
```

即可运行程序





5. 操作流程

请依次按照步骤点击文件上传、词法分析、语法分析，终端显示分析过程信息。

在语法分析栏中点击下方三个按钮，分别展示 LR1 分析表、入栈规约过程和语法树。

所有过程文件存储在 `.../source_code/PROCESS_FILES` 文件夹中。

文件结构为：

```
D:.
│  README.md
│  struct.txt
│  词法分析器与语法分析器实现报告.pdf
│  
├─graphviz
│      windows_10_cmake_Release_graphviz-install-9.0.0-win64.exe
│      
└─source_code
    │  display.py					# tkinter 展示接口
    │  LexAnalyzer.py				# 词法分析器实现
    │  Lex_main.py					# 词法分析器调用
    │  SyntaxAnalyzer.py			# 语法分析器实现
    │  Syn_main.py					# 语法分析器调用
    │  test1.cpp					
    │  test2.cpp
    │  test3.txt
    │  test_err.cpp
    │  
    ├─.idea
    │  │  .gitignore
    │  │  deployment.xml
    │  │  encodings.xml
    │  │  lexical_analyzer_py.iml
    │  │  misc.xml
    │  │  modules.xml
    │  │  workspace.xml
    │  │  
    │  └─inspectionProfiles
    │          profiles_settings.xml
    │          Project_Default.xml
    │          
    ├─PROCESS_FILES
    │      0.LexSynCheckList.txt	# 终结符、非终结符映射表
    │      1.LexResultIndex.txt		
    │      2.LexResultList.txt
    │      3.LexResultWord.txt		# 词法分析结果
    │      4.SyntaxGrammar.txt
    │      5.LR1Table.txt			# LR1 表
    │      6.AnalyzeProcess.txt		# 规约过程
    │      backup_production.txt
    │      backup_Table.txt
    │      backup_tb_s_r.txt
    │      LexResultWord_display.txt
    │      LR1_info_tuple.txt
    │      SyntaxGrammar_display.txt # 输入语法
    │      SynTaxTree.dot
    │      SynTaxTree.png			# 语法树图片
    │      
    └─__pycache__
            LexAnalyzer.cpython-39.pyc
            Lex_main.cpython-39.pyc
            SyntaxAnalyzer.cpython-39.pyc
            Syn_main.cpython-39.pyc
            

```

