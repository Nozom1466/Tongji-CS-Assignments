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
* @func:   展示产生的token的列表
* @para:
* @return:
*/
void Show::showToken()
{
    this->ui->centralwidget->setWindowTitle("Token显示");
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
* @func:   展示词法DFA
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
* @func:   //展示Action goto表
* @para:
* @return:
*/
void Show::showTable()
{
    this->ui->centralwidget->setWindowTitle("Action Goto表显示");
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
        // 读取文件内容并解析为行
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
* @func:   //展示语法分析树
* @para:
* @return:
*/
void Show::showTree()
{
    this->ui->centralwidget->setWindowTitle("Action Goto表显示");
    // 创建一个 QGraphicsView
    myQGraphicsView* view = new myQGraphicsView(this->ui->centralwidget);

    // 创建一个 QGraphicsScene
    QGraphicsScene* scene = new QGraphicsScene(this->ui->centralwidget);

    QPixmap pixmap(TREE_PNG_FILE);

    scene->addPixmap(pixmap);
    view->setDragMode(QGraphicsView::ScrollHandDrag);// 设置鼠标拖拽格式

    // 将场景设置给 QGraphicsView
    view->setScene(scene);

    if (this->layout != NULL) {
        delete this->layout;
    }
    // 创建一个垂直布局管理器
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // 将 QGraphicsView 添加到布局中
    this->layout->addWidget(view);

    // 设置主窗口的布局
    this->ui->centralwidget->setLayout(this->layout);

}

/**
* @func:   //展示语法DFA
* @para:
* @return:
*/
void Show::showParseDFA()
{


    this->ui->centralwidget->setWindowTitle("LR(1) DFA显示");
    // 创建一个 QGraphicsView
    myQGraphicsView* view = new myQGraphicsView(this->ui->centralwidget);

    // 创建一个 QGraphicsScene
    QGraphicsScene* scene = new QGraphicsScene(this->ui->centralwidget);

    QPixmap pixmap(PARSE_DFA_PNG_FILE);

    scene->addPixmap(pixmap);
    view->setDragMode(QGraphicsView::ScrollHandDrag);// 设置鼠标拖拽格式

    // 将场景设置给 QGraphicsView
    view->setScene(scene);

    if (this->layout != NULL) {
        delete this->layout;
    }
    // 创建一个垂直布局管理器
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // 将 QGraphicsView 添加到布局中
    this->layout->addWidget(view);

    // 设置主窗口的布局
    this->ui->centralwidget->setLayout(this->layout);

}

/**
* @func:   //展示归约过程
* @para:
* @return:
*/
void Show::showProcess()
{
    this->ui->centralwidget->setWindowTitle("归约过程显示");
    QTextBrowser* textBrowser = new QTextBrowser(this->ui->centralwidget);
    // 设置 TextBrowser 的内容
    QFile file(REDUCTION_PROCESS_FILE);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        // 读取文件内容并写入 TextBrowser
        textBrowser->setPlainText(in.readAll());
        file.close();
    }
    else {
        // 处理文件打开失败的情况
        textBrowser->setPlainText("Error: Unable to open the file.");
    }
    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // 将 TextBrowser 添加到布局中
    this->layout->addWidget(textBrowser);

    // 设置主窗口的布局
    this->ui->centralwidget->setLayout(this->layout);

}


void Show::showQuaternion()
{
    //this->ui->centralwidget->setWindowTitle("归约过程显示");
    QTextBrowser* textBrowser = new QTextBrowser(this->ui->centralwidget);
    // 设置 TextBrowser 的内容
    QFile file(QUATERNION_FILE);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        // 读取文件内容并写入 TextBrowser
        textBrowser->setPlainText(in.readAll());
        file.close();
    }
    else {
        // 处理文件打开失败的情况
        textBrowser->setPlainText("Error: Unable to open the file.");
    }
    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // 将 TextBrowser 添加到布局中
    this->layout->addWidget(textBrowser);

    // 设置主窗口的布局
    this->ui->centralwidget->setLayout(this->layout);
}


void Show::showFunc()
{
    //this->ui->centralwidget->setWindowTitle("归约过程显示");
    QTextBrowser* textBrowser = new QTextBrowser(this->ui->centralwidget);
    // 设置 TextBrowser 的内容
    QFile file(FUNC_TABLE_FILE);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        // 读取文件内容并写入 TextBrowser
        textBrowser->setPlainText(in.readAll());
        file.close();
    }
    else {
        // 处理文件打开失败的情况
        textBrowser->setPlainText("Error: Unable to open the file.");
    }
    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // 将 TextBrowser 添加到布局中
    this->layout->addWidget(textBrowser);

    // 设置主窗口的布局
    this->ui->centralwidget->setLayout(this->layout);
}


void Show::showVar()
{
    //this->ui->centralwidget->setWindowTitle("归约过程显示");
    QTextBrowser* textBrowser = new QTextBrowser(this->ui->centralwidget);
    // 设置 TextBrowser 的内容
    QFile file(VAR_TABLE_FILE);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        // 读取文件内容并写入 TextBrowser
        textBrowser->setPlainText(in.readAll());
        file.close();
    }
    else {
        // 处理文件打开失败的情况
        textBrowser->setPlainText("Error: Unable to open the file.");
    }
    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // 将 TextBrowser 添加到布局中
    this->layout->addWidget(textBrowser);

    // 设置主窗口的布局
    this->ui->centralwidget->setLayout(this->layout);
}


void Show::showObjectCode()
{
    //this->ui->centralwidget->setWindowTitle("归约过程显示");
    QTextBrowser* textBrowser = new QTextBrowser(this->ui->centralwidget);
    // 设置 TextBrowser 的内容
    QFile file(OBJECT_CODE_FILE);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        // 读取文件内容并写入 TextBrowser
        textBrowser->setPlainText(in.readAll());
        file.close();
    }
    else {
        // 处理文件打开失败的情况
        textBrowser->setPlainText("Error: Unable to open the file.");
    }
    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // 将 TextBrowser 添加到布局中
    this->layout->addWidget(textBrowser);

    // 设置主窗口的布局
    this->ui->centralwidget->setLayout(this->layout);
}

//@name  : 构造函数
//@para  : 
//@return: 
myQGraphicsView::myQGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
}

//@name  : 鼠标滚轮事件
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

//@name  : 画面缩小
//@para  : 
//@return: 
void myQGraphicsView::shrink()
{
    zoom(1.0 / 1.2);
}
//@name  : 画面增大
//@para  : 
//@return: 
void myQGraphicsView::expand()
{
    zoom(1.2);
}
//@name  : 画面改变
//@para  : 
//@return: 
void myQGraphicsView::zoom(double scaleFactor)
{
    //缩放函数
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.1 || factor > 50)
        return;
    scale(scaleFactor, scaleFactor);
}

//@name  : 刷新界面
//@para  : 
//@return: 
void myQGraphicsView::refresh()
{
    this->viewport()->update();
}

