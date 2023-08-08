#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myopenglwidget.h" // 包含OpenGL显示
#include "myhighlighter.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 文本框
    void newDocument();
    void open();
    bool save();
    void saveAs();
    void close();
    void print();
    void exit();
    void copy();
    void cut();
    void paste();
    void undo();
    void redo();
    void selectFont();
    void setFontBold(bool bold);
    void setFontUnderline(bool underline);
    void setFontItalic(bool italic);
    void about();
    //OpenGL显示
    void runMyGL();
    void stopMyGL();

private:
    Ui::MainWindow *ui;
    QFont font; // 字体

    QString defaultFragPath = QDir::currentPath()+ QString("/Shaders/FALSE.frag");
    QString currentFile; // 目前打开的文本文件

    MyOpenGLWidget *myopenglwidget = NULL; // OpenGL显示

    MyHighlighter *highlighter;

};

#endif // MAINWINDOW_H
