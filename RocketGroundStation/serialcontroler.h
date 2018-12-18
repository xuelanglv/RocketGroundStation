#ifndef SERIALCONTROLER_H
#define SERIALCONTROLER_H

#include <QtSerialPort/QSerialPortInfo>
#include <qDebug>
#include <QtSerialPort>
#include <QIODevice>
#include <QMessageBox>
#include <map>


class serialControler:public QObject
{
    Q_OBJECT

public:
    serialControler();
    //serialControler(QSerialPortInfo& serialInfo);
    bool openSerialPort();
    bool sendData(float center_x, float center_y, float track_x, float track_y, float width, float height); // 发送信号

    bool isOpen(); // 串口是否开启
    QSerialPort* serial; // 串口

    bool receiveData(); // 接收信号

private:
    uint8_t Dec2Hex(uint8_t number);
    uint8_t calcCheckSum(const uint8_t& R1,const uint8_t& R2);
    std::vector<float> calcRValues(float center_x, float center_y, float track_x, float track_y, float width, float height); // 处理目标数据


    std::map<uint8_t, uint8_t> int2hex{{0,0x00}, {1,0x01}, {2,0x02}, {3,0x03}, {4,0x04},
                                 {5,0x05}, {6,0x06}, {7,0x07}, {8,0x08}, {9,0x09},
                                 {10, 0x0A}, {11, 0x0B}, {12, 0x0C}, {13, 0x0D}, {14, 0x0E}, {15, 0x0F}};

    bool open_flag;
};

#endif // SERIALCONTROLER_H
