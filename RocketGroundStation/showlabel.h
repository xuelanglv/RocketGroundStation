#ifndef SHOWLABEL_H
#define SHOWLABEL_H
#pragma execution_character_set("utf-8")
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QPen>
#include<QPoint>

class ShowLabel : public QLabel
{
    Q_OBJECT
public:
    ShowLabel(QWidget *parent);
    ~ShowLabel();
    void getImage(QImage& frame);
    void getImageRect(QRect rect);
    bool closeBoxFlag(); // 锁定检测框
    bool openBoxFlag(); // 解除锁定框
    QRect getBoxRect(); // 获得检测框
    bool isBoxSelect(); // 检测框是否已经设置好
    void setBoxRect(QRect rect); // 设置检测框

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e); // 鼠标按下事件
    void mouseMoveEvent(QMouseEvent *e); // 鼠标移动
    void mouseReleaseEvent(QMouseEvent *e); // 鼠标释放

private:

    QRect getBoxRect(const QPoint &topLeft, const QPoint &bottomRight);
    QRect box_rect;
    QRect image_rect;
    QImage image;
    //static QColor lineColor; // 辅助线颜色
    bool box_flag; // 检测框标志
    QPoint begin_point;
    QPoint end_point;
};

//QColor ShowLabel::lineColor(106,254,60);
#endif // SHOWLABEL_H
