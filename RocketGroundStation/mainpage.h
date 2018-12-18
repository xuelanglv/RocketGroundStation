#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <Windows.h>
#include <windef.h>
#include <setupapi.h>
#include <QCoreApplication>
#include <QMainWindow>
#include <QScreen>
#include <QPixmap>
#include <QTimer>
#include <qDebug>
#include <QWindow>
#include <QList>
#include <QCameraInfo>
#include <QCamera>
#include <QCameraImageCapture>
#include <QPainter>
#include <QDateTime>
#include <QCameraViewfinderSettings>
#include <QSerialPortInfo>
#include <thread>
#include <fstream>
#include "showlabel.h"
#include "tracker.h"
#include "kcftracker.hpp"
#include "common.h"
#include "serialcontroler.h"

namespace Ui {
class MainPage;
}

class MainPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = 0);
    ~MainPage();
    void textSender(const QString& text);


signals:
    void textSend(const QString& text);



private slots:
    void showScreen(int, QImage);
    void selectCamrea(QAction*); // 选择摄像头
    void selectSerialPort(QAction*); // 选择端口
    void getFrame(); // 获得帧
    void openInsurance();
    void openLock();  // 锁定按钮
    void openLaunch();
    void openScram();  // 打开急停按钮
    void textReceiver(const QString& text);


private:
    void trackerInit();
    void boxInit(const QImage& image, const QRect& select_roi);
    void rect2File(float center_x, float center_y, float track_x, float track_y, float width, float height); // 将检测的框输出到文件

    Ui::MainPage *ui;
    QTimer* timer;
    QScreen *screen;
    QCamera *camera;//摄像头
    QCameraImageCapture * imageCapture;//截图部件
    QList<QCameraInfo> cameras;
    bool insuranceStatus;
    bool lockStatus;
    bool launchStatus;
    bool scramStatus;
    KCFTracker tracker;   // 追踪器
    QImage oriFrame;
    QVector<QString> textVector;
    std::ofstream outFile;
    std::string fileName;
    std::vector<float> RValues; // 遥控器阻值
    serialControler portControler; // 串口控制类
};

#endif // MAINPAGE_H
