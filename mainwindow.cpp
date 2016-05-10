#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "httpdownload.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

connect(ui->pushButton, SIGNAL (released()),this, SLOT (handleButton()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton()
{
    // pictureDate = QDate::currentDate().toString(QString("yyyy-MM-dd");
    //MyNetworkAccessManager myNetworkAccessManager;
    //myNetworkAccessManager.GetPicture("2016-01-01");
    HttpDownload w;
    w.setWindowTitle("Http Download");
    w.show();
}
