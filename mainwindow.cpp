#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QFontDialog>
#include <QShortcut>
#include <QPushButton>
#include "myhighlighter.h"

// 构造函数部分初始化及槽链接
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Hello shader");

//    QString text = QString("File -> New File / Open File");
//    ui->textEdit->setText(text); // 默认提示
//    ui->textEdit->setReadOnly(true); // 默认未打开文本，不可编辑

    // ##### 打开默认着色器 ##### //
    QString filename = QDir::currentPath()+ QString("/Shaders/FALSE.frag"); // 文件保存路径
    currentFile = filename; // 文件路径传给成员变量
    QFile file(filename); // 新建打开文件
    // 只读打开文件
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        qDebug() << "[error] 无法打开文件:" << file.errorString();
        QMessageBox::warning(NULL, "error", "无法打开文件:"+ file.errorString(), QMessageBox::Yes);
        currentFile.clear(); // 文件路径清空
    }
    setWindowTitle(filename); // 更新窗口标题为文件名
    if(!currentFile.isEmpty())
    {
        ui->textEdit->setReadOnly(false); // 文本框可编辑
    }
    QTextStream in(&file); // 文件读入
    QString text = in.readAll(); // 读所有内容
    ui->textEdit->setText(text); // 显示到textdit窗口
    file.close();

    // 槽链接
    connect(ui->actionNew_File, &QAction::triggered, this, &MainWindow::newDocument);
    connect(ui->actionOpen_2, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionSave_File, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_as_2, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionPrint_2, &QAction::triggered, this, &MainWindow::print);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exit);
    connect(ui->actionExit_2, &QAction::triggered, this, &MainWindow::exit);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copy);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::cut);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::paste);
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::redo);
    connect(ui->actionFont, &QAction::triggered, this, &MainWindow::selectFont);
    connect(ui->actionBold, &QAction::triggered, this, &MainWindow::setFontBold);
    connect(ui->actionUnderLine, &QAction::triggered, this, &MainWindow::setFontUnderline);
    connect(ui->actionItalic, &QAction::triggered, this, &MainWindow::setFontItalic);
    connect(ui->actionAbout_2, &QAction::triggered, this, &MainWindow::about);

    // 显示OpenGL
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(runMyGL())); // 按钮
    connect(ui->actionRun_2, &QAction::triggered, this, &MainWindow::runMyGL); // Bar

    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(stopMyGL())); // 按钮
    connect(ui->actionStop, &QAction::triggered, this, &MainWindow::stopMyGL); // Bar

    // QT添加快捷键
    ui->actionAbout_2->setShortcut(QKeySequence("Ctrl+h"));
    ui->actionSave_File->setShortcut(QKeySequence("Ctrl+s"));
    ui->actionUndo->setShortcut(QKeySequence("Ctrl+z"));
    ui->actionRedo->setShortcut(QKeySequence("Ctrl+Shift+z"));

    // Disable menu actions for unavailable features
#if !defined(QT_PRINTSUPPORT_LIB) || !QT_CONFIG(printer)
    ui->actionPrint_2->setEnabled(false);
#endif

#if !QT_CONFIG(clipboard)
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);
#endif

//    //背景和选区颜色
//    QPalette pt = palette();
//    pt.setBrush(QPalette::Text, Qt::white);
//    pt.setBrush(QPalette::Base, Qt::black);
//    pt.setBrush(QPalette::Highlight, Qt::gray);
//    pt.setBrush(QPalette::HighlightedText, Qt::NoBrush);
//    setPalette(pt);
    //语法高亮
    highlighter = new MyHighlighter(ui->textEdit->document());
}

MainWindow::~MainWindow()
{
    delete ui;
    if(myopenglwidget != NULL)
    {
        delete myopenglwidget;
        myopenglwidget = NULL;
    }
}

// ######################################################################################
// 函数功能实现
// ######################################################################################
void MainWindow::newDocument()
{
    currentFile.clear(); // 文件路径清空
    ui->textEdit->setText(QString()); // 文本窗口清空

    QString filename = QFileDialog::getSaveFileName(this, "Open File", "./", 0, 0, QFileDialog::DontUseNativeDialog);
    currentFile = filename;
    QFile file(filename);

    // 打开文件
    if(!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        qDebug() << "[Warning] Cannot new file:" << file.errorString();
        QMessageBox::information(NULL, "error", "无法打开文件:"+ file.errorString(), QMessageBox::Yes);
        currentFile.clear(); // 文件路径清空
        currentFile = defaultFragPath;
        return;
    }
    setWindowTitle(filename); // 更新窗口标题为文件名
    if(!currentFile.isEmpty())
    {
        ui->textEdit->setReadOnly(false); // 文本框可编辑
    }
    file.close();
}

void MainWindow::open()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "./", 0, 0, QFileDialog::DontUseNativeDialog); // 文件保存路径
    currentFile = filename; // 文件路径传给成员变量
    QFile file(filename); // 新建打开文件
    // 只读打开文件
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        qDebug() << "[error] 无法打开文件:" << file.errorString();
        QMessageBox::warning(NULL, "error", "无法打开文件:"+ file.errorString(), QMessageBox::Yes);
        currentFile.clear(); // 文件路径清空
        currentFile = defaultFragPath;
    }
    setWindowTitle(filename); // 更新窗口标题为文件名
    if(!currentFile.isEmpty())
    {
        ui->textEdit->setReadOnly(false); // 文本框可编辑
    }
    QTextStream in(&file); // 文件读入
    QString text = in.readAll(); // 读所有内容

    ui->textEdit->setText(text); // 显示到textdit窗口

    file.close();
}

bool MainWindow::save()
{
    // 判断文件是否已经存在
    if(currentFile.isEmpty())
    {
//        filename = QFileDialog::getSaveFileName(this, "Save");
//        currentFile = filename; // 没有文件名就给个Save
        qDebug() << "[Warning] 请先点击New新建一个文件，或者点击Open打开一个文件！";
        QMessageBox::warning(NULL, "Warning", "请先点击New新建一个文件，或者点击Open打开一个文件！", QMessageBox::Yes);
        return false;
    }

    QFile file(currentFile);
    if(!file.open(QIODevice::WriteOnly | QFile:: Text))
    {
        qDebug() << "[Warning] 无法保存文件！";
        QMessageBox::warning(this, "Warning", "无法保存文件: " + file.errorString());
        return false;
    }
    setWindowTitle(currentFile);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text; // 文本框内容写入文件中
    QMessageBox::information(NULL, "success", "已保存", QMessageBox::Yes);
    file.close();
    return true;
}

void MainWindow::saveAs()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save as",0, 0, 0, QFileDialog::DontUseNativeDialog);
    QFile  file(filename);

    if(!file.open(QIODevice::WriteOnly | QFile:: Text))
    {
        qDebug() << "[Warning] 无法保存文件！";
        QMessageBox::warning(this, "Warning", "无法保存文件: " + file.errorString());
        return;
    }
    currentFile = filename;
    setWindowTitle(filename);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    QMessageBox::information(NULL, "success", "已保存", QMessageBox::Yes);
    out << text;
    file.close();
}

void MainWindow::close()
{

}

void MainWindow::print()
{
    #if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
        QPrinter printDev;
    #if QT_CONFIG(printdialog)
        QPrintDialog dialog(&printDev, this);
        if (dialog.exec() == QDialog::Rejected)
            return;
    #endif // QT_CONFIG(printdialog)
        ui->textEdit->print(&printDev);
    #endif // QT_CONFIG(printer)
}

void MainWindow::exit()
{
    QCoreApplication::quit();
}

void MainWindow::copy()
{
#if QT_CONFIG(clipboard)
    ui->textEdit->copy(); // 复制文本窗口内容
#endif
}

void MainWindow::cut()
{
#if QT_CONFIG(clipboard)
    ui->textEdit->cut(); // 剪切文本窗口内容
#endif
}

void MainWindow::paste()
{
#if QT_CONFIG(clipboard)
    ui->textEdit->paste(); // 粘贴文本窗口内容
#endif
}

void MainWindow::undo()
{
    ui->textEdit->undo();
}

void MainWindow::redo()
{
    ui->textEdit->redo();
}


void MainWindow::selectFont()
{
    bool fontSelected;
    font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
        ui->textEdit->setFont(font);
}

void MainWindow::setFontBold(bool bold)
{
    if(!bold)
    {
        font.setBold(false);
    }
    else
    {
        font.setBold(true);
    }
    ui->textEdit->setFont(font);
}

void MainWindow::setFontUnderline(bool underline)
{
    if(!underline)
    {
        font.setUnderline(false);
    }
    else
    {
        font.setUnderline(true);
    }
    ui->textEdit->setFont(font);
}

void MainWindow::setFontItalic(bool italic)
{
    if(!italic)
    {
        font.setItalic(false);
    }
    else
    {
        font.setItalic(true);
    }
    ui->textEdit->setFont(font);
}

void MainWindow::about()
{
    QMessageBox msgBox;
    msgBox.setBaseSize(200,200);
    msgBox.setText("GLSL shader text editor & runner.");
    msgBox.setInformativeText("A QT test by CS.");
//    QIcon *icon = new QIcon("/home/liao/sda2/myProject/NotepadCS/cs.jpg");
//    msgBox.setWindowIcon(*icon);

//    msgBox.setStyleSheet("image:url(:/home/liao/sda2/myProject/NotepadCS/cs.ico)");
    msgBox.setStandardButtons(QMessageBox::Cancel);
    msgBox.exec();
}

// 调用OpenGL显示
void MainWindow::runMyGL()
{
    if(!save()) // 自动保存
    {
        return ;
    }

    if(myopenglwidget == NULL)
    {
        myopenglwidget = new MyOpenGLWidget(currentFile);
        myopenglwidget->show();
    }
    else
    {
        delete myopenglwidget;
        myopenglwidget = NULL;
        myopenglwidget = new MyOpenGLWidget(currentFile);
        myopenglwidget->show();
    }
//    myopenglwidget->runPaint = true;

}

void MainWindow::stopMyGL()
{
    qDebug() << "stop";
    if(myopenglwidget == NULL)
    {
        QMessageBox::warning(NULL, "warning", "未打开项目shader文件", QMessageBox::Yes);
        return;
    }
    if(myopenglwidget != NULL)
    {
        delete myopenglwidget;
        myopenglwidget = NULL;
    }
//    myopenglwidget->runPaint = false;
}










