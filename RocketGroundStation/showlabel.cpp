#include "showlabel.h"

ShowLabel::ShowLabel(QWidget *parent):QLabel(parent)
{
    box_flag = true;
    begin_point = QPoint(-1,-1);
    end_point = QPoint(-1,-1);
}


ShowLabel::~ShowLabel()
{
    qDebug() << "del ShowLabel";
}

void ShowLabel::getImage(QImage& frame)
{
    image = frame;
}

void ShowLabel::getImageRect(QRect rect)
{
    image_rect = rect;
}


bool ShowLabel::closeBoxFlag()
{
    if(box_flag){
        box_flag = false;
        return true;
    }
    else
        return false;
}

bool ShowLabel::openBoxFlag()
{
    if(!box_flag){
        box_flag = true;
        return true;
    }
    else
        return false;
}

QRect ShowLabel::getBoxRect()
{
    return box_rect;
}

bool ShowLabel::isBoxSelect()
{
    if(end_point !=  QPoint(-1,-1) || begin_point != end_point)
        return true;
    else
        return false;
}

void ShowLabel::setBoxRect(QRect rect)
{
    box_rect = rect;
}

void ShowLabel::paintEvent(QPaintEvent *)
{
    const int x = image_rect.width();
    const int y = image_rect.height();
    this->resize(x,y);
    QPainter p(this);
    QColor lineColor(106,254,60); 
    p.setPen(QPen(lineColor, 1));   
    p.drawPixmap(0,0,QPixmap::fromImage(image));
    p.drawLine(0,y/2, x, y/2); // 水平线
    p.drawLine(x/2,0, x/2, y); // 垂直线

    // 画选择框
    QColor selectColor(254,106,60);
    p.setPen(QPen(selectColor, 1));
    p.drawRect(box_rect);
}


void ShowLabel::mousePressEvent(QMouseEvent *e)
{
    if(box_flag){
        begin_point = e->pos();
    }
}


void ShowLabel::mouseMoveEvent(QMouseEvent *e)
{
    if(box_flag){
        end_point = e->pos();
        box_rect = getBoxRect(begin_point, end_point);
        update();
    }
}

void ShowLabel::mouseReleaseEvent(QMouseEvent *e)
{
    if(box_flag){
        end_point = e->pos();
        box_rect = getBoxRect(begin_point, end_point);
        update();
    }
}

QRect ShowLabel::getBoxRect(const QPoint &topLeft, const QPoint &bottomRight)
{
    int minx, miny, width, height;
    if(topLeft.x() <= bottomRight.x()){
        minx = topLeft.x();
        height = bottomRight.x() - topLeft.x();
    }
    else{
        minx = bottomRight.x();
        height = topLeft.x() - bottomRight.x();
    }
    if(topLeft.y() <= bottomRight.y()){
        miny = topLeft.y();
        width = bottomRight.y() - topLeft.y();
    }
    else{
        miny = bottomRight.y();
        width = topLeft.y() - bottomRight.y();
    }
    return QRect(minx, miny, height, width);
}
