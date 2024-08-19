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

	//�����������ķ�
	connect(this->ui->action_Code, &QPushButton::clicked, this, &myCompiler::loadSourceCode);
	connect(this->ui->action_Grammar, &QPushButton::clicked, this, &myCompiler::loadGrammar);

	//�����������
	connect(this->ui->action_Run, &QPushButton::clicked, [=]() {
		consoleLog("�����ĵȴ�");
		if (!this->grammarFlag) {
			consoleLog("û��֧�ֱ������Ч�ķ�", MSG_ERROR);
		}
		else {
			if (!this->loadGrammarFlag) consoleLog("��ָ���ķ��ļ���ʹ��Ĭ���ķ��ļ�", MSG_WARNING);
			this->compile();
		}
	});
	connect(this->ui->action_Restart, &QPushButton::clicked, this, &myCompiler::restart);

	//չʾ�ʷ�dfa
	connect(this->ui->action_DFA, &QPushButton::clicked, [=]() {
		if (this->lexerFlag == 0) {
			consoleLog("����δ��ɱ������", MSG_ERROR);
		}
		else {
			this->pageShow->showDFA();
			this->pageShow->show();
		}
	});


	// չʾaction goto��
	connect(this->ui->action_Table, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("����δ��ɱ������", MSG_ERROR);
		}
		else {
			this->pageShow->showTable();
			this->pageShow->show();
		}
	});


	// չʾ�ʷ�token
	connect(this->ui->action_Token, &QPushButton::clicked, [=]() {
		if (this->lexerFlag == 0) {
			consoleLog("����δ��ɱ������", MSG_ERROR);
		}
		else {
			this->pageShow->showToken();
			this->pageShow->show();
		}
	});

	// չʾ��Լ����
	connect(this->ui->action_Process, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("����δ��ɱ������", MSG_ERROR);
		}
		else {
			this->pageShow->showProcess();
			this->pageShow->show();
		}
	});


	// չʾ�﷨������
	connect(this->ui->action_Tree, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("����δ��ɱ������", MSG_ERROR);
		}
		else if (this->treeFlag == 0) {
			consoleLog("�����﷨��δ����", MSG_ERROR);
		}
		else {
			this->pageShow->showTree();
			this->pageShow->show();
		}
	});

	// չʾ�﷨������
	connect(this->ui->actionLR_1_DFA, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("����δ��ɱ������", MSG_ERROR);
		}
		else if (this->dfaFlag == 0) {
			consoleLog("����δ���� ItemSet DFA", MSG_ERROR);
		}
		else {
			this->pageShow->showParseDFA();
			this->pageShow->show();
		}
	});

	// չʾ�м�������ɵ���Ԫʽ
	connect(this->ui->action_Quaternion, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("����δ��ɱ������", MSG_ERROR);
		}
		else {
			this->pageShow->showQuaternion();
			this->pageShow->show();
		}
	});
	// չʾ������
	connect(this->ui->action_Var, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("����δ��ɱ������", MSG_ERROR);
		}
		else {
			this->pageShow->showVar();
			this->pageShow->show();
		}
	});
	// չʾ������
	connect(this->ui->action_Func, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("����δ��ɱ������", MSG_ERROR);
		}
		else {
			this->pageShow->showFunc();
			this->pageShow->show();
		}
	});

	// չʾĿ�����
	connect(this->ui->actionMIPS_code, &QPushButton::clicked, [=]() {
		if (this->compileFlag == 0) {
			consoleLog("����δ��ɱ������", MSG_ERROR);
		}
		else {
			this->pageShow->showObjectCode();
			this->pageShow->show();
		}
	});

	consoleLog(">>>>����������<<<<");
}

myCompiler::~myCompiler() {}

void myCompiler::loadGrammar()
{
	consoleLog("�����ĵȴ��ķ�����");
	QString fileName = QFileDialog::getOpenFileName(this, "�����ļ�", ".", "files(*.txt)");
	if (!fileName.isEmpty()) {
		this->grammarFile = fileName;
		this->loadGrammarFlag = 1;
		consoleLog("�ķ��ļ�����ɹ�", MSG_SUCCESS);
		this->my_lr1Grammar = new GrammaAnalysis;
		if (!readGrammar()) {
			consoleLog("����ʧ��", MSG_ERROR);
		}
		consoleLog("�����ķ��ļ��ɹ�", MSG_SUCCESS);
	}
	else {
		consoleLog("�ķ�����ʧ��", MSG_ERROR);
	}
}

void myCompiler::loadSourceCode()
{
	QString fileName = QFileDialog::getOpenFileName(this, "�����ļ�", ".", "files(*.txt *.cpp *.c)");
	if (!fileName.isEmpty()) {
		this->codeFile = fileName;
		fstream fcode;
		fcode.open((string)fileName.toLocal8Bit());
		if (fcode.is_open() == 0) {
			consoleLog(("Դ��" + fileName + "��ʧ��").toStdString(), MSG_ERROR);
			return;
		}
		this->ui->textEdit->clear();
		while (!fcode.eof() && fcode) {
			string line;
			getline(fcode, line);
			this->ui->textEdit->append(QString::fromLocal8Bit(line.c_str()));
		}
		fcode.close();
		consoleLog("Դ�뵼��ɹ�", MSG_SUCCESS);
	}
	else {
		consoleLog("Դ�뵼��ʧ��", MSG_ERROR);
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

	//���ı�����������
	QString codeInput = this->ui->textEdit->toPlainText();
	if (codeInput.size() > 0) {
		QFile file(CODE_FILE);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream out(&file);
			out << codeInput;
			file.close();
			consoleLog("���뱸�ݳɹ�", MSG_SUCCESS);
		}
		else {
			consoleLog("���棺���뱸��ʧ��", MSG_WARNING);
		}
	}
	else {
		consoleLog("����Դ��Ϊ��", MSG_ERROR);
		return;
	}


	//�ʷ�����
	consoleLog("�ʷ�������ʼ");
	this->my_lexer = new lexer;
	int lexer_error = this->my_lexer->lexicalAnalysis(CODE_FILE);

	if (lexer_error == SUCCESS) {
		consoleLog("�ʷ������ɹ�", MSG_SUCCESS);
	}
	else if (lexer_error == LEXICAL_ANALYSIS_ERROR) {
		end = clock();
		consoleLog("�ʷ�����ʧ��\n������־:");

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
			consoleLog("���󣺴�����־�ļ���ʧ��");
			return;
		}
		printTime(int(end - start));
		consoleLog("����ʧ��", MSG_ERROR);
		return;
	}
	else {
		consoleLog("Դ�뵼��ʧ��", MSG_ERROR);
		return;
	}
	consoleLog("�ʷ������ɹ�", MSG_SUCCESS);
	this->lexerFlag = 1;


	consoleLog("�﷨������ʼ");
	int lr1_error = my_lr1Grammar->Forward(my_lexer);

	if (lr1_error == SUCCESS) {
		this->my_lr1Grammar->SemanticProcess.PrintCodeTable();
		this->my_lr1Grammar->SemanticProcess.PrintVarTable();
		this->my_lr1Grammar->SemanticProcess.PrintFuncTable();

		consoleLog("�Ż� & ����Ŀ����뿪ʼ");
		this->my_optimizer = new Optimizer(this->my_lr1Grammar->SemanticProcess.VarTable, 
			                               this->my_lr1Grammar->SemanticProcess.FuncTable, 
			                               this->my_lr1Grammar->SemanticProcess.MiddleCodeTable);
		this->my_objector = new ObjectCodeGenerator(my_optimizer->VarTable, my_optimizer->FuncTable, my_optimizer->MiddleCodeTable, my_optimizer->CodeBlock, OBJECT_CODE_FILE);
		my_objector->Forward();
	}

	end = clock();

	if (lr1_error == ACTION_ERROR) {
		consoleLog("ACTION�����", MSG_ERROR);
	}
	else if (lr1_error == GOTO_ERROR) {
		consoleLog("GOTO�����", MSG_ERROR);
	}
	else if (lr1_error == REDUCTION_PROCESS_FILE_OPEN_ERROR) {
		consoleLog("��Լ���̼�¼ʧ��",  MSG_ERROR);
		return;
	}
	else if (lr1_error == PARSE_ERROR_FILE_OPEN_ERROR) {
		consoleLog("�﷨���������¼ʧ��", MSG_ERROR);
		return;
	}
	else if (lr1_error == SEMANTIC_ERROR) {
		consoleLog("�����������", MSG_ERROR);
	}
	else {
		consoleLog("�﷨�������", MSG_SUCCESS);
		printTime(int(end - start));
		consoleLog("����ɹ�\n", MSG_SUCCESS);

		this->compileFlag = 1;
		this->errorFlag = 0;

		
		if (lr1_error == TREE_DOT_FILE_OPEN_ERROR) {
			consoleLog("LR(1)��������", MSG_ERROR);
			return;
		}
		else {
			if (QMessageBox::Yes == QMessageBox::question(this, "ע��", "�﷨����������ʱ��ϳ����Ƿ��������", QMessageBox::Yes | QMessageBox::No))
			{
				consoleLog("�﷨����ͼ��ʼ");
				int a = system("dot -Tpng results/Parse_Tree.dot -o picture/Parse_Tree.png");
				if (a != 0) {
					consoleLog("δ��⵽Graphviz�������﷨����������ʧ��", MSG_ERROR);
					return;
				}
				consoleLog("�﷨���������ɳɹ�", MSG_SUCCESS);
				this->treeFlag = 1;
			}
		}

		if (my_lr1Grammar->printParseDFA() != SUCCESS) {
			consoleLog("ItemSet DFA ����ʧ��", MSG_ERROR);
		}
		else {
			if (QMessageBox::Yes == QMessageBox::question(this, "ע��", "��Ŀ��DFA����ʱ��ϳ����Ƿ��������", QMessageBox::Yes | QMessageBox::No))
			{
				consoleLog("ItemSet DFA ��ͼ��ʼ");
				system("dot -Tpng results/Parse_DFA.dot -o picture/Parse_DFA.png");
				consoleLog("ItemSet DFA ��ͼ�ɹ�", MSG_SUCCESS);
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
			consoleLog("��־�ļ���ʧ��", MSG_ERROR);
			return;
		}
		if (lr1_error == SEMANTIC_ERROR) {
			QFile file2(SEMANTIC_ERROR_FILE);

			if (file2.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QTextStream in(&file2);
				// ��ȡ�ļ����ݲ�д�� TextBrowser
				in.setCodec("UTF-8");
				consoleLog(in.readAll().toStdString(), MSG_ERROR);
				file2.close();
			}
			else {
				consoleLog("��־�ļ���ʧ��", MSG_ERROR);
				return;
			}
		}
		printTime(int(end - start));
		consoleLog("����ʧ��", MSG_ERROR);
		return;
	}
}

int myCompiler::readGrammar()
{
	consoleLog("��ȡ�ķ�����...");
	this->grammarFlag = 0;

	int msg = FAILED;
	if (this->my_lr1Grammar->ProduceFormsGenerate((string)this->grammarFile.toLocal8Bit()) == SUCCESS) {
		this->my_lr1Grammar->BuildDFA();
		msg = this->my_lr1Grammar->BuildActionGo();
	}

	if (msg == SUCCESS) {
		consoleLog("�ķ��ļ�����", MSG_SUCCESS);
	}
	else if (msg == GRAMMAR_ERROR) {
		consoleLog("�ķ�����!\n������־:");

		QFile file(GRAMMAR_ERROR_FILE);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&file);
			in.setCodec("UTF-8");
			consoleLog(in.readAll().toStdString(), MSG_ERROR);
			file.close();
		}
		else {
			consoleLog("�����޷���Ŀ���ļ�", MSG_ERROR);
			return 0;
		}
		QMessageBox::critical(this, "����", "�ķ�����");
		return 0;
	}
	else if (msg == GRAMMAR_FILE_OPEN_ERROR) {
		consoleLog("�ķ��ļ�����ʧ��", MSG_ERROR);
		return 0;
	}
	this->my_lr1Grammar->PrintActionGoToFile();
	consoleLog("�ķ��ѳɹ�����", MSG_SUCCESS);
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
	string allt = "��ʱ " + to_string(ms) + "ms";
	consoleLog(allt);
}

void myCompiler::restart()
{
	//���
	this->ui->textBrowser->clear();
	this->ui->textEdit->clear();

	consoleLog("������...");
	this->grammarFile = DEFAULT_GRAMMAR;
	this->loadGrammarFlag = 0;
	this->codeFile = "";
	this->compileFlag = 0;
	this->treeFlag = 0;
	this->lexerFlag = 0;
	this->errorFlag = 0;
	this->dfaFlag = 0;

	consoleLog(">>>>�����������ɹ�<<<<");
	this->my_lr1Grammar = new GrammaAnalysis;
	int ans = readGrammar();
	if (ans == 0) {
		consoleLog("����ʧ��", MSG_ERROR);
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
	blockFormat.setForeground(color); // �����ı���ɫ

	// Ӧ�ø�ʽ������
	cursor.setBlockFormat(blockFormat);
}