#include "mainwindow.h"
#include <QApplication>
#include "httpdownload.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    HttpDownload w;
    w.setWindowTitle("Http Download");
    w.show();

    return a.exec();
}
