#include "serialcontroler.h"

serialControler::serialControler()
{
    open_flag = false;
}

bool serialControler::openSerialPort()
{
    if(serial->open(QIODevice::ReadWrite)){
        serial->setBaudRate(QSerialPort::Baud115200); // 设置波特率
        open_flag = true;
        return true;
    }
    else{
        serial->close();
        return false;
    }
}

bool serialControler::sendData(float center_x, float center_y, float track_x, float track_y, float width, float height)
{
    std::vector<float> dataList;
    dataList = calcRValues(center_x,center_y,track_x,track_y,width,height);
    float R_AB, R_W, R_updown, R_leftright, D_n0, D_n1;
    uint8_t dec_0, dec_1;
    uint8_t Uart_send[4];
    Uart_send[0] = 0xAA; // 包头
    R_AB = 10000; // 总阻值
    R_W = 52; // 变阻器头阻值
    // 阻值转换为相对应的十六进制数
    R_updown = dataList.at(0);
    R_leftright = dataList.at(1);
    D_n0 = 256-(R_updown - R_W)*256/R_AB; // 获得阻值对应的倍数
    D_n1 = 256-(R_leftright - R_W)*256/R_AB;
    dec_0 = static_cast<uint8_t>(D_n0); // 强制类型转换
    dec_1 = static_cast<uint8_t>(D_n1);
    Uart_send[1] = Dec2Hex(dec_0); // 转换成十六进制
    Uart_send[2] = Dec2Hex(dec_1);

    Uart_send[3] = calcCheckSum(Uart_send[1], Uart_send[2]);
    QByteArray data(reinterpret_cast<const char*>(Uart_send), 4); // 传输数据
    qDebug() << "write data:" << data << Uart_send[3];
    serial->write(data);
    return true;
}

bool serialControler::receiveData()
{
    QByteArray data = serial->readAll();
    QString str = QString::fromLatin1(data.data());
    qDebug() << "C" <<str;
    return true;
}

bool serialControler::isOpen()
{
    if(open_flag && serial->isOpen())
        return true;
    else
        return false;
}


uint8_t serialControler::Dec2Hex(uint8_t number)
// 十进制转十六进制
{
    uint8_t binary[8], i, Hex, hex0, hex1;

    for(i=0; i<8; i++){ // i==8保证位数填0，如00000001
        binary[i] = number%2;
        number /= 2;
    }
    hex0 = binary[0] + binary[1]*2 + binary[2]*4 + binary[3]*8;
    hex1 = binary[4] + binary[5]*2 + binary[6]*4 + binary[7]*8;
    Hex = (0 & 0xF0) + int2hex[hex0];
    Hex = (0 & 0x0F) + (int2hex[hex1]<<4); // 高4位置0，获得的值左移向高4位再赋值过去
    qDebug() << "S" << Hex << hex0 << hex1;
    return Hex;
}

uint8_t serialControler::calcCheckSum(const uint8_t& R1,const uint8_t& R2)
{
    uint8_t checksum;
    checksum = R1 + R2 + 0xAA;
    qDebug() << "J" << checksum << "B";
    return checksum;
}

std::vector<float> serialControler::calcRValues(float center_x, float center_y, float track_x, float track_y, float width, float height)
{
    std::vector<float> dataList;
    float R1, R2, offset_x, offset_y;
    offset_x = (track_x - center_x)/(2*center_x);
    offset_y = (track_y - center_y)/(2*center_y);
    R1 = 1772*offset_x+3440;
    R2=-1806*offset_y+2798;
    dataList.push_back(R1);
    dataList.push_back(R2);
    qDebug() << R1 << R2;
    return dataList;
}

