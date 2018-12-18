#include "mainpage.h"
#include <QApplication>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainPage w;
    w.show();
    return a.exec();
}
