#include "CCompiler.h"

myCompiler::myCompiler(QWidget* parent) : QMainWindow(parent)
{
	this->ui = new Ui::myCompilerClass;
	this->ui->setupUi(this);
	this->compileFlag = 0;
	this->loadGrammarFlag = 0;
	this->treeFlag = 0;
	this->lexerFlag = 0;
	this->errorFlag = 0;
	this->errorLine = 0;
	this->dfaFlag = 0;
	this->codeFile = "";
	this->pageShow = new Show;
	this->my_lr1Grammar = new GrammaAnalysis;
	this->grammarFile = DEFAULT_GRAMMAR;

	//导入代码或者文法
	connect(this->ui->action_Code, &QPushButton::clicked, this, &myCompiler::loadSourceCode);
	connect(this->ui->action_Grammar, &QPushButton::clicked, this, &myCompiler::loadGrammar);

	//编译或者重置
	connect(this->ui->action_Run, &QPushButton::clicked, [=]() {
		consoleLog("请耐心等待");
		if (!this->grammarFlag) {
			consoleLog("没有支持编译的有效文法", MSG_ERROR);
		}
		else {
			if (!this->loadGrammarFlag) consoleLog("无指定文法文件，使用默认文法文件", MSG_WARNING);
			this->compile();
		}
	});
	connect(this->ui->action_Restart, &QPushButton::clicked, this, &myCompiler::restart);

	//展示词法dfa
	connect(this->ui->action_DFA, &QPushButton::clicked, [=]() {
		if (this->lexerFlag == 0) {
			consoleLog("错误：未完成编译过程", MSG_ERROR);
		}
		else {
			this->pageShow->showDFA();
			this->pageShow->show();
		}
	});


	// 展示action goto表
	connect(this->ui->action_Table, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("错误：未完成编译过程", MSG_ERROR);
		}
		else {
			this->pageShow->showTable();
			this->pageShow->show();
		}
	});


	// 展示词法token
	connect(this->ui->action_Token, &QPushButton::clicked, [=]() {
		if (this->lexerFlag == 0) {
			consoleLog("错误：未完成编译过程", MSG_ERROR);
		}
		else {
			this->pageShow->showToken();
			this->pageShow->show();
		}
	});

	// 展示规约过程
	connect(this->ui->action_Process, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("错误：未完成编译过程", MSG_ERROR);
		}
		else {
			this->pageShow->showProcess();
			this->pageShow->show();
		}
	});


	// 展示语法分析树
	connect(this->ui->action_Tree, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("错误：未完成编译过程", MSG_ERROR);
		}
		else if (this->treeFlag == 0) {
			consoleLog("错误：语法树未生成", MSG_ERROR);
		}
		else {
			this->pageShow->showTree();
			this->pageShow->show();
		}
	});

	// 展示语法分析树
	connect(this->ui->actionLR_1_DFA, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("错误：未完成编译过程", MSG_ERROR);
		}
		else if (this->dfaFlag == 0) {
			consoleLog("错误：未生成 ItemSet DFA", MSG_ERROR);
		}
		else {
			this->pageShow->showParseDFA();
			this->pageShow->show();
		}
	});

	// 展示中间代码生成的四元式
	connect(this->ui->action_Quaternion, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("错误：未完成编译过程", MSG_ERROR);
		}
		else {
			this->pageShow->showQuaternion();
			this->pageShow->show();
		}
	});
	// 展示变量表
	connect(this->ui->action_Var, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("错误：未完成编译过程", MSG_ERROR);
		}
		else {
			this->pageShow->showVar();
			this->pageShow->show();
		}
	});
	// 展示函数表
	connect(this->ui->action_Func, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("错误：未完成编译过程", MSG_ERROR);
		}
		else {
			this->pageShow->showFunc();
			this->pageShow->show();
		}
	});

	// 展示目标代码
	connect(this->ui->actionMIPS_code, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("错误：未完成编译过程", MSG_ERROR);
		}
		else {
			this->pageShow->showObjectCode();
			this->pageShow->show();
		}
	});

	consoleLog(">>>>编译器启动<<<<");
}

myCompiler::~myCompiler() {}

void myCompiler::loadGrammar()
{
	consoleLog("请耐心等待文法导入");
	QString fileName = QFileDialog::getOpenFileName(this, "导入文件", ".", "files(*.txt)");
	if (!fileName.isEmpty()) {
		this->grammarFile = fileName;
		this->loadGrammarFlag = 1;
		consoleLog("文法文件导入成功", MSG_SUCCESS);
		this->my_lr1Grammar = new GrammaAnalysis;
		if (!readGrammar()) {
			consoleLog("编译失败", MSG_ERROR);
		}
		consoleLog("导入文法文件成功", MSG_SUCCESS);
	}
	else {
		consoleLog("文法导入失败", MSG_ERROR);
	}
}

void myCompiler::loadSourceCode()
{
	QString fileName = QFileDialog::getOpenFileName(this, "导入文件", ".", "files(*.txt *.cpp *.c)");
	if (!fileName.isEmpty()) {
		this->codeFile = fileName;
		fstream fcode;
		fcode.open((string)fileName.toLocal8Bit());
		if (fcode.is_open() == 0) {
			consoleLog(("源码" + fileName + "打开失败").toStdString(), MSG_ERROR);
			return;
		}
		this->ui->textEdit->clear();
		while (!fcode.eof() && fcode) {
			string line;
			getline(fcode, line);
			this->ui->textEdit->append(QString::fromLocal8Bit(line.c_str()));
		}
		fcode.close();
		consoleLog("源码导入成功", MSG_SUCCESS);
	}
	else {
		consoleLog("源码导入失败", MSG_ERROR);
	}
}

void myCompiler::compile()
{
	time_t start, end;
	start = clock();

	this->compileFlag = 0;
	this->treeFlag = 0;
	this->lexerFlag = 0;
	if (this->errorFlag == 1) {
		this->showErrorLine(this->errorLine, Qt::white);
	}

	//读文本输入框的内容
	QString codeInput = this->ui->textEdit->toPlainText();
	if (codeInput.size() > 0) {
		QFile file(CODE_FILE);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream out(&file);
			out << codeInput;
			file.close();
			consoleLog("代码备份成功", MSG_SUCCESS);
		}
		else {
			consoleLog("警告：代码备份失败", MSG_WARNING);
		}
	}
	else {
		consoleLog("错误：源码为空", MSG_ERROR);
		return;
	}


	//词法分析
	consoleLog("词法分析开始");
	this->my_lexer = new lexer;
	int lexer_error = this->my_lexer->lexicalAnalysis(CODE_FILE);

	if (lexer_error == SUCCESS) {
		consoleLog("词法分析成功", MSG_SUCCESS);
	}
	else if (lexer_error == LEXICAL_ANALYSIS_ERROR) {
		end = clock();
		consoleLog("词法分析失败\n错误日志:");

		this->showErrorLine(this->my_lexer->error_line, Qt::red);
		this->errorLine = this->my_lexer->error_line;
		this->errorFlag = 1;

		QFile file(ERROR_LIST_FILE);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&file);
			in.setCodec("UTF-8");
			consoleLog(in.readAll().toStdString(), MSG_ERROR);
			file.close();
		}
		else {
			consoleLog("错误：错误日志文件打开失败");
			return;
		}
		printTime(int(end - start));
		consoleLog("编译失败", MSG_ERROR);
		return;
	}
	else {
		consoleLog("源码导入失败", MSG_ERROR);
		return;
	}
	consoleLog("词法分析成功", MSG_SUCCESS);
	this->lexerFlag = 1;


	consoleLog("语法分析开始");
	int lr1_error = my_lr1Grammar->Forward(my_lexer);

	if (lr1_error == SUCCESS) {
		this->my_lr1Grammar->SemanticProcess.PrintCodeTable();
		this->my_lr1Grammar->SemanticProcess.PrintVarTable();
		this->my_lr1Grammar->SemanticProcess.PrintFuncTable();

		consoleLog("优化 & 生成目标代码开始");
		this->my_optimizer = new Optimizer(this->my_lr1Grammar->SemanticProcess.VarTable, 
			                               this->my_lr1Grammar->SemanticProcess.FuncTable, 
			                               this->my_lr1Grammar->SemanticProcess.MiddleCodeTable);
		this->my_objector = new ObjectCodeGenerator(my_optimizer->VarTable, my_optimizer->FuncTable, my_optimizer->MiddleCodeTable, my_optimizer->CodeBlock, OBJECT_CODE_FILE);
		my_objector->Forward();
	}

	end = clock();

	if (lr1_error == ACTION_ERROR) {
		consoleLog("ACTION表错误", MSG_ERROR);
	}
	else if (lr1_error == GOTO_ERROR) {
		consoleLog("GOTO表错误", MSG_ERROR);
	}
	else if (lr1_error == REDUCTION_PROCESS_FILE_OPEN_ERROR) {
		consoleLog("规约过程记录失败",  MSG_ERROR);
		return;
	}
	else if (lr1_error == PARSE_ERROR_FILE_OPEN_ERROR) {
		consoleLog("语法分析错误记录失败", MSG_ERROR);
		return;
	}
	else if (lr1_error == SEMANTIC_ERROR) {
		consoleLog("语义分析错误", MSG_ERROR);
	}
	else {
		consoleLog("语法分析完成", MSG_SUCCESS);
		printTime(int(end - start));
		consoleLog("编译成功\n", MSG_SUCCESS);

		this->compileFlag = 1;
		this->errorFlag = 0;

		
		if (lr1_error == TREE_DOT_FILE_OPEN_ERROR) {
			consoleLog("LR(1)分析错误", MSG_ERROR);
			return;
		}
		else {
			if (QMessageBox::Yes == QMessageBox::question(this, "注意", "语法分析树生成时间较长，是否继续生成", QMessageBox::Yes | QMessageBox::No))
			{
				consoleLog("语法树绘图开始");
				int a = system("dot -Tpng results/Parse_Tree.dot -o picture/Parse_Tree.png");
				if (a != 0) {
					consoleLog("未检测到Graphviz环境，语法分析树生成失败", MSG_ERROR);
					return;
				}
				consoleLog("语法分析树生成成功", MSG_SUCCESS);
				this->treeFlag = 1;
			}
		}

		if (my_lr1Grammar->printParseDFA() != SUCCESS) {
			consoleLog("ItemSet DFA 保存失败", MSG_ERROR);
		}
		else {
			if (QMessageBox::Yes == QMessageBox::question(this, "注意", "项目集DFA生成时间较长，是否继续生成", QMessageBox::Yes | QMessageBox::No))
			{
				consoleLog("ItemSet DFA 绘图开始");
				system("dot -Tpng results/Parse_DFA.dot -o picture/Parse_DFA.png");
				consoleLog("ItemSet DFA 绘图成功", MSG_SUCCESS);
				this->dfaFlag = 1;
			}
		}

		
	}
	if (lr1_error != SUCCESS) {
		this->showErrorLine(this->my_lr1Grammar->ErrorLine, Qt::red);
		this->errorLine = this->my_lr1Grammar->ErrorLine;
		this->errorFlag = 1;
		QFile file(PARSE_ERROR_FILE);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&file);
			in.setCodec("UTF-8");
			consoleLog(in.readAll().toStdString(), MSG_ERROR);
			file.close();
		}
		else {
			consoleLog("日志文件打开失败", MSG_ERROR);
			return;
		}
		if (lr1_error == SEMANTIC_ERROR) {
			QFile file2(SEMANTIC_ERROR_FILE);

			if (file2.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QTextStream in(&file2);
				// 读取文件内容并写入 TextBrowser
				in.setCodec("UTF-8");
				consoleLog(in.readAll().toStdString(), MSG_ERROR);
				file2.close();
			}
			else {
				consoleLog("日志文件打开失败", MSG_ERROR);
				return;
			}
		}
		printTime(int(end - start));
		consoleLog("编译失败", MSG_ERROR);
		return;
	}
}

int myCompiler::readGrammar()
{
	consoleLog("读取文法声明...");
	this->grammarFlag = 0;

	int msg = FAILED;
	if (this->my_lr1Grammar->ProduceFormsGenerate((string)this->grammarFile.toLocal8Bit()) == SUCCESS) {
		this->my_lr1Grammar->BuildDFA();
		msg = this->my_lr1Grammar->BuildActionGo();
	}

	if (msg == SUCCESS) {
		consoleLog("文法文件无误", MSG_SUCCESS);
	}
	else if (msg == GRAMMAR_ERROR) {
		consoleLog("文法错误!\n错误日志:");

		QFile file(GRAMMAR_ERROR_FILE);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&file);
			in.setCodec("UTF-8");
			consoleLog(in.readAll().toStdString(), MSG_ERROR);
			file.close();
		}
		else {
			consoleLog("错误：无法打开目标文件", MSG_ERROR);
			return 0;
		}
		QMessageBox::critical(this, "错误", "文法错误");
		return 0;
	}
	else if (msg == GRAMMAR_FILE_OPEN_ERROR) {
		consoleLog("文法文件读入失败", MSG_ERROR);
		return 0;
	}
	this->my_lr1Grammar->PrintActionGoToFile();
	consoleLog("文法已成功读入", MSG_SUCCESS);
	this->grammarFlag = 1;
	return 1;
}

void myCompiler::consoleLog(string msg, int msg_type)
{
	auto cur_text_color = this->ui->textBrowser->textColor();
	
	if (msg_type == MSG_NORMAL) {
		this->ui->textBrowser->setTextColor(Qt::black);
	} 
	else if (msg_type == MSG_ERROR) {
		this->ui->textBrowser->setTextColor(Qt::darkRed);
	}
	else if (msg_type == MSG_SUCCESS) {
		this->ui->textBrowser->setTextColor(Qt::darkGreen);
	}
	else if (msg_type == MSG_WARNING) {
		this->ui->textBrowser->setTextColor(Qt::darkYellow);
	}

	this->ui->textBrowser->append(QString::fromStdString(msg));
	this->ui->textBrowser->setTextColor(cur_text_color);
}

void myCompiler::printTime(int ms)
{
	string allt = "耗时 " + to_string(ms) + "ms";
	consoleLog(allt);
}

void myCompiler::restart()
{
	//清空
	this->ui->textBrowser->clear();
	this->ui->textEdit->clear();

	consoleLog("重启中...");
	this->grammarFile = DEFAULT_GRAMMAR;
	this->loadGrammarFlag = 0;
	this->codeFile = "";
	this->compileFlag = 0;
	this->treeFlag = 0;
	this->lexerFlag = 0;
	this->errorFlag = 0;
	this->dfaFlag = 0;

	consoleLog(">>>>编译器重启成功<<<<");
	this->my_lr1Grammar = new GrammaAnalysis;
	int ans = readGrammar();
	if (ans == 0) {
		consoleLog("编译失败", MSG_ERROR);
	}
}

void myCompiler::showErrorLine(int n, QColor color)
{
	QTextCursor cursor = this->ui->textEdit->textCursor();
	cursor.movePosition(QTextCursor::Start);
	for (int i = 0; i < n - 1; ++i) {
		cursor.movePosition(QTextCursor::NextBlock);
	}

	QTextBlockFormat blockFormat = cursor.blockFormat();
	//blockFormat.setBackground(color);
	blockFormat.setForeground(color); // 设置文本颜色

	// 应用格式到整行
	cursor.setBlockFormat(blockFormat);
}