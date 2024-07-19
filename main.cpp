#include "VideoUI.h"
#include <QtWidgets/QApplication>
#include"Audio.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VideoUI w;
    w.show();
    return a.exec();
}
