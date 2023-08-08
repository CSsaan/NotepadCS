#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QFile>
#include <QTimer>
#include<sys/time.h>
#include <QMouseEvent> // 鼠标拖动
#include <QWheelEvent> //鼠标滚轮
#include <QDir>

struct AVFrame;

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions //QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
    explicit MyOpenGLWidget(QString paramFragPath, QWidget *parent = nullptr);
    ~MyOpenGLWidget();
    void savepicture();
    void setMode(int mode);
    // 计时函数
    struct timeval G_tv_begin, G_tv_end;
    void time_begin();
    void time_end();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    // 鼠标拖动事件
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override; //鼠标滚轮滚动

private:
    void checkGLRrror(const char *msg);
    void myGetLocation();
    void myUniformData();
    int updateImage();

    // 鼠标拖动事件
    QPoint mousePoint;
    bool mouse_press;

    const QString fL_video_YUYV = QDir::currentPath() + QString("/YUYV/yuyv.yuv");
    QOpenGLShaderProgram *program = NULL;

    GLuint texs_yuyvID[1] = {0};
    GLuint texs_yuyvloc[1] = {0};

    QFile m_file;
    QTimer m_timer;

    // uniform_loc
    GLuint frameWH_loc = 0;

    // uniform value
    int width = 1920;
    int height = 1080;

    unsigned char* imageData;
    int save_wid = 1920;
    int save_hei = 1080;

    GLuint VAO, VBO;

    QByteArray buf; // 视频
    // PBO
    int m_FrameIndex;
    GLuint pboIds[2] = {0};
    GLubyte* bufPtr0 = NULL;
    GLubyte* bufPtr1 = NULL;

    unsigned char * buff = NULL;

public:
//    bool runPaint = false;
    QString fragPath = QDir::currentPath()+ QString("/Shaders/FALSE.frag");

signals:

public slots:
    void cleanup();

};

#endif // MYOPENGLWIDGET_H

