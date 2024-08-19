#include "Display.h"

Show::Show(QWidget* parent) : QMainWindow(parent)
{
    this->layout = NULL;
    this->ui = new Ui::ShowWindow;
    this->ui->setupUi(this);
}


Show::~Show()
{
}

/**
* @func:   չʾ������token���б�
* @para:
* @return:
*/
void Show::showToken()
{
    this->ui->centralwidget->setWindowTitle("Token��ʾ");
    QTextBrowser* textBrowser = new QTextBrowser(this->ui->centralwidget);
    QFile file(TOKEN_LIST_FILE);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        textBrowser->setPlainText(in.readAll());
        file.close();
    }
    else {
        textBrowser->setPlainText("Error: Unable to open the file.");
    }
    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);
    this->layout->addWidget(textBrowser);
    this->ui->centralwidget->setLayout(this->layout);
}

/**
* @func:   չʾ�ʷ�DFA
* @para:
* @return:
*/
void Show::showDFA()
{
    myQGraphicsView* view = new myQGraphicsView(this->ui->centralwidget);
    QGraphicsScene* scene = new QGraphicsScene(this->ui->centralwidget);
    QPixmap pixmap(DFA_PNG_FILE);

    scene->addPixmap(pixmap);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setScene(scene);

    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);
    this->layout->addWidget(view);
    this->ui->centralwidget->setLayout(this->layout);
}

/**
* @func:   //չʾAction goto��
* @para:
* @return:
*/
void Show::showTable()
{
    this->ui->centralwidget->setWindowTitle("Action Goto����ʾ");
    QTableWidget* tableWidget = new QTableWidget();

    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    QFile file(Table_FILE);
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec(codec);
        // ��ȡ�ļ����ݲ�����Ϊ��
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(',');
            int row = tableWidget->rowCount();
            tableWidget->insertRow(row);
            tableWidget->setColumnCount(fields.size());
            for (int column = 0; column < fields.size(); column++) {
                QTableWidgetItem* item = new QTableWidgetItem(fields.at(column));
                tableWidget->setItem(row, column, item);
            }
        }
        tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

        file.close();
    }
    else {
        QTableWidgetItem* errorItem = new QTableWidgetItem("Error: Unable to open the file.");
        tableWidget->setItem(0, 0, errorItem);
    }
    this->layout->addWidget(tableWidget);
    this->ui->centralwidget->setLayout(this->layout);
}

/**
* @func:   //չʾ�﷨������
* @para:
* @return:
*/
void Show::showTree()
{
    this->ui->centralwidget->setWindowTitle("Action Goto����ʾ");
    // ����һ�� QGraphicsView
    myQGraphicsView* view = new myQGraphicsView(this->ui->centralwidget);

    // ����һ�� QGraphicsScene
    QGraphicsScene* scene = new QGraphicsScene(this->ui->centralwidget);

    QPixmap pixmap(TREE_PNG_FILE);

    scene->addPixmap(pixmap);
    view->setDragMode(QGraphicsView::ScrollHandDrag);// ���������ק��ʽ

    // ���������ø� QGraphicsView
    view->setScene(scene);

    if (this->layout != NULL) {
        delete this->layout;
    }
    // ����һ����ֱ���ֹ�����
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // �� QGraphicsView ��ӵ�������
    this->layout->addWidget(view);

    // ���������ڵĲ���
    this->ui->centralwidget->setLayout(this->layout);

}

/**
* @func:   //չʾ�﷨DFA
* @para:
* @return:
*/
void Show::showParseDFA()
{


    this->ui->centralwidget->setWindowTitle("LR(1) DFA��ʾ");
    // ����һ�� QGraphicsView
    myQGraphicsView* view = new myQGraphicsView(this->ui->centralwidget);

    // ����һ�� QGraphicsScene
    QGraphicsScene* scene = new QGraphicsScene(this->ui->centralwidget);

    QPixmap pixmap(PARSE_DFA_PNG_FILE);

    scene->addPixmap(pixmap);
    view->setDragMode(QGraphicsView::ScrollHandDrag);// ���������ק��ʽ

    // ���������ø� QGraphicsView
    view->setScene(scene);

    if (this->layout != NULL) {
        delete this->layout;
    }
    // ����һ����ֱ���ֹ�����
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // �� QGraphicsView ��ӵ�������
    this->layout->addWidget(view);

    // ���������ڵĲ���
    this->ui->centralwidget->setLayout(this->layout);

}

/**
* @func:   //չʾ��Լ����
* @para:
* @return:
*/
void Show::showProcess()
{
    this->ui->centralwidget->setWindowTitle("��Լ������ʾ");
    QTextBrowser* textBrowser = new QTextBrowser(this->ui->centralwidget);
    // ���� TextBrowser ������
    QFile file(REDUCTION_PROCESS_FILE);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        // ��ȡ�ļ����ݲ�д�� TextBrowser
        textBrowser->setPlainText(in.readAll());
        file.close();
    }
    else {
        // �����ļ���ʧ�ܵ����
        textBrowser->setPlainText("Error: Unable to open the file.");
    }
    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // �� TextBrowser ��ӵ�������
    this->layout->addWidget(textBrowser);

    // ���������ڵĲ���
    this->ui->centralwidget->setLayout(this->layout);

}


void Show::showQuaternion()
{
    //this->ui->centralwidget->setWindowTitle("��Լ������ʾ");
    QTextBrowser* textBrowser = new QTextBrowser(this->ui->centralwidget);
    // ���� TextBrowser ������
    QFile file(QUATERNION_FILE);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        // ��ȡ�ļ����ݲ�д�� TextBrowser
        textBrowser->setPlainText(in.readAll());
        file.close();
    }
    else {
        // �����ļ���ʧ�ܵ����
        textBrowser->setPlainText("Error: Unable to open the file.");
    }
    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // �� TextBrowser ��ӵ�������
    this->layout->addWidget(textBrowser);

    // ���������ڵĲ���
    this->ui->centralwidget->setLayout(this->layout);
}


void Show::showFunc()
{
    //this->ui->centralwidget->setWindowTitle("��Լ������ʾ");
    QTextBrowser* textBrowser = new QTextBrowser(this->ui->centralwidget);
    // ���� TextBrowser ������
    QFile file(FUNC_TABLE_FILE);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        // ��ȡ�ļ����ݲ�д�� TextBrowser
        textBrowser->setPlainText(in.readAll());
        file.close();
    }
    else {
        // �����ļ���ʧ�ܵ����
        textBrowser->setPlainText("Error: Unable to open the file.");
    }
    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // �� TextBrowser ��ӵ�������
    this->layout->addWidget(textBrowser);

    // ���������ڵĲ���
    this->ui->centralwidget->setLayout(this->layout);
}


void Show::showVar()
{
    //this->ui->centralwidget->setWindowTitle("��Լ������ʾ");
    QTextBrowser* textBrowser = new QTextBrowser(this->ui->centralwidget);
    // ���� TextBrowser ������
    QFile file(VAR_TABLE_FILE);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        // ��ȡ�ļ����ݲ�д�� TextBrowser
        textBrowser->setPlainText(in.readAll());
        file.close();
    }
    else {
        // �����ļ���ʧ�ܵ����
        textBrowser->setPlainText("Error: Unable to open the file.");
    }
    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // �� TextBrowser ��ӵ�������
    this->layout->addWidget(textBrowser);

    // ���������ڵĲ���
    this->ui->centralwidget->setLayout(this->layout);
}


void Show::showObjectCode()
{
    //this->ui->centralwidget->setWindowTitle("��Լ������ʾ");
    QTextBrowser* textBrowser = new QTextBrowser(this->ui->centralwidget);
    // ���� TextBrowser ������
    QFile file(OBJECT_CODE_FILE);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        // ��ȡ�ļ����ݲ�д�� TextBrowser
        textBrowser->setPlainText(in.readAll());
        file.close();
    }
    else {
        // �����ļ���ʧ�ܵ����
        textBrowser->setPlainText("Error: Unable to open the file.");
    }
    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // �� TextBrowser ��ӵ�������
    this->layout->addWidget(textBrowser);

    // ���������ڵĲ���
    this->ui->centralwidget->setLayout(this->layout);
}

//@name  : ���캯��
//@para  : 
//@return: 
myQGraphicsView::myQGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
}

//@name  : �������¼�
//@para  : 
//@return: 
void myQGraphicsView::wheelEvent(QWheelEvent* event)
{
    auto step = event->angleDelta().y();
    QPointF cursorPoint = event->pos();
    QPointF scenePos = this->mapToScene(cursorPoint.x(), cursorPoint.y());
    qreal factor = 1 + ((qreal)step / (qreal)viewport()->height()); scale(factor, factor);

    QPointF viewPoint = this->transform().map(scenePos);
    horizontalScrollBar()->setValue(viewPoint.x() - cursorPoint.x());
    verticalScrollBar()->setValue(viewPoint.y() - cursorPoint.y());

}

//@name  : ������С
//@para  : 
//@return: 
void myQGraphicsView::shrink()
{
    zoom(1.0 / 1.2);
}
//@name  : ��������
//@para  : 
//@return: 
void myQGraphicsView::expand()
{
    zoom(1.2);
}
//@name  : ����ı�
//@para  : 
//@return: 
void myQGraphicsView::zoom(double scaleFactor)
{
    //���ź���
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.1 || factor > 50)
        return;
    scale(scaleFactor, scaleFactor);
}

//@name  : ˢ�½���
//@para  : 
//@return: 
void myQGraphicsView::refresh()
{
    this->viewport()->update();
}

