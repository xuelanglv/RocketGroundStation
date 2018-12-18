#include "mainpage.h"
#include "ui_mainpage.h"

MainPage::MainPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainPage)
{
    // 成员变量初始化
    ui->setupUi(this);
    camera=new QCamera;//摄像头
    imageCapture=new QCameraImageCapture(camera);// 图片获取容器
    QTimer *timer = new QTimer(this);
    cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {
        ui->cameraMenu->addAction(cameraInfo.description());
    }
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->serialPortMenu->addAction(info.portName());
    }
    insuranceStatus = false;
    lockStatus = false;
    launchStatus = false;
    trackerInit(); // 跟踪器初始化
    fileName = "Rect.txt";
    //setWindowFlags(Qt::FramelessWindowHint);

    // 建立连接
    connect(timer, SIGNAL(timeout()), this, SLOT(getFrame()));
    connect(ui->cameraMenu,SIGNAL(triggered(QAction*)),this,SLOT(selectCamrea(QAction*))); // 这里有待改进，应该重写信号
    connect(ui->serialPortMenu,SIGNAL(triggered(QAction*)),this,SLOT(selectSerialPort(QAction*)));
    connect(ui->insuranceBtn, SIGNAL(clicked()), this, SLOT(openInsurance()));
    connect(ui->lockBtn, SIGNAL(clicked()), this, SLOT(openLock()));
    connect(ui->launchBtn, SIGNAL(clicked()), this, SLOT(openLaunch()));
    connect(ui->scram_btn, SIGNAL(clicked()), this, SLOT(openScram()));
    connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(showScreen(int, QImage)));
    connect(this, SIGNAL(textSend(QString)), this, SLOT(textReceiver(QString)));
   if(imageCapture->isCaptureDestinationSupported(QCameraImageCapture::CaptureToBuffer)) // 是否支持输出到缓冲区
        imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer); // 保存到内存缓冲区

    // 开始工作
    timer->start(0);
    camera->start();
}

MainPage::~MainPage()
{
    delete ui;
    delete camera;
}

void MainPage::showScreen(int, QImage image)
{
    oriFrame = image;
    ui->screen->getImage(image);
    ui->screen->getImageRect(image.rect());
    if(lockStatus){
        cv::Mat frame = Common::QImage2cvMat(image);
        cv::Rect result = tracker.update(frame);
        //rect2File(320.0, 240.0, result.x + result.width / 2, result.y + result.height/2, result.width, result.height);
        // 串口发送数据
        portControler.sendData(320.0, 240.0, result.x + result.width / 2, result.y + result.height/2, result.width, result.height);
        //Sleep(1000);
        portControler.receiveData(); // 接收数据
        ui->screen->setBoxRect(QRect(result.x, result.y, result.width, result.height));
    }
    ui->screen->update();
}

void MainPage::selectCamrea(QAction *act)
{
    camera->stop();
    disconnect(imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(showScreen(int, QImage))); // 取消之前的连接
    foreach (const QCameraInfo &cameraInfo, cameras) {
        if(cameraInfo.description() == act->text()){
            camera = new QCamera(cameraInfo);
            camera->start();
            QCameraViewfinderSettings set;   // 临时设置，控制摄像头的分辨率
            set.setResolution(640, 480);
            camera->setViewfinderSettings(set);
            imageCapture= new QCameraImageCapture(camera);
            connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(showScreen(int, QImage)));
            break;
        }
    }
}

void MainPage::selectSerialPort(QAction *act)
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if(info.portName() == act->text()){
            QSerialPortInfo infoPortName = QSerialPortInfo(info.portName());
            portControler.serial = new QSerialPort(infoPortName, this);
            if(portControler.openSerialPort()){
                //connect(portControler.serial, SIGNAL(readyRead()), portControler, SLOT(receiveData()));
            }else{ // 设置串口错误
                QMessageBox::critical(this, tr("Error"), portControler.serial->errorString());
            }
            break;
        }
    }
}

void MainPage::getFrame()
{
    if(!camera->error()){
        imageCapture->capture();
    }
    else{
        qDebug() << camera->error();
    }
}

void MainPage::openInsurance()
{

    if(!insuranceStatus){
        insuranceStatus = true;
        ui->insuranceBtn->setStyleSheet("QPushButton{"
                                        "background-image:url(:/Resources/button_active.png);"
                                        "border-width: 30px;"
                                        "border-radius: 30px;"
                                        "color:white;"
                                        "}");
        textSender("保险打开！");
    }
}

void MainPage::openLock()
{
    if(!portControler.isOpen()){
       QMessageBox::critical(this, tr("Error"), "请选择串口！");
       return;
    }
    if(!lockStatus){ // 锁定
        if(insuranceStatus && ui->screen->isBoxSelect()){ // 保险打开，并且锁定对象
            lockStatus = true;
            ui->lockBtn->setStyleSheet("QPushButton{"
                                       "background-image:url(:/Resources/button_active.png);"
                                       "border-width: 30px;"
                                       "border-radius: 30px;"
                                       "color:white;"
                                       "}");
            boxInit(oriFrame, ui->screen->getBoxRect());
            textSender("目标锁定！");
        }else{
            textSender("请选定目标框！！！");
        }
    }else{ // 取消锁定
        ui->lockBtn->setStyleSheet("QPushButton{"
                                   "background-image:url(:/Resources/button_inactive.png);"
                                   "border-width: 30px;"
                                   "border-radius: 30px;"
                                   "color:white;"
                                   "}");
        lockStatus = false;
        trackerInit(); // 重新设置追踪器
        textSender("取消锁定！");
    }
}

void MainPage::openLaunch()
{
    if(!launchStatus){
        if(insuranceStatus && lockStatus){

            launchStatus = true;
            ui->launchBtn->setStyleSheet("QPushButton{"
                                         "background-image:url(:/Resources/button_active.png);"
                                         "border-width: 30px;"
                                         "border-radius: 30px;"
                                         "color:white;"
                                         "}");
            textSender("发射灭火弹！");
        }
    }
}

void MainPage::openScram()
{
    if(!scramStatus){
        if(launchStatus){
            scramStatus = true;
            ui->scram_btn->setStyleSheet("QPushButton{"
                                         "background-image:url(:/Resources/button_active.png);"
                                         "border-width: 30px;"
                                         "border-radius: 30px;"
                                         "color:white;"
                                         "}");
            textSender("灭火弹紧急制动！");
        }
    }
}

void MainPage::textReceiver(const QString &text)
{
    ui->textBrowser->append(text);
}

void MainPage::trackerInit()
{
    bool HOG = false;
    bool FIXEDWINDOW = true;
    bool MULTISCALE = false;
    //bool SILENT = true;
    bool LAB = true;
    tracker = KCFTracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);
}

void MainPage::boxInit(const QImage &image,const QRect& select_roi)
{
    double xMin = select_roi.x();
    double yMin = select_roi.y();
    double width = select_roi.width();
    double height = select_roi.height();
    cv::Mat frame = Common::QImage2cvMat(image);
    qDebug() << frame.data;
    tracker.init( cv::Rect2d(xMin, yMin, width, height), frame);
}

void MainPage::rect2File(float center_x, float center_t, float track_x, float track_y, float width, float height)
{

    outFile.open(fileName, std::ofstream::trunc);
    outFile << center_x << "," << center_t << "," << track_x << "," << track_y << "," << width<< "," << height;
    outFile.close();
}

void MainPage::textSender(const QString& text)
{
    QDateTime time = QDateTime::currentDateTime();
    QString current_time = time.toString(" yyyy-MM-dd hh:mm:ss ");
    QString info_text= current_time + text;
    emit textSend(info_text); // 发射信号（内容）
}
