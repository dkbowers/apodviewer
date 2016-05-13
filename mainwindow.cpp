#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDebug>
#include <QTemporaryDir>
//#include "dbmanager.h"


#include <QtCore/QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug() << "MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow)";

    ui->setupUi(this);

    ui->calendarWidget->setMinimumDate(QDate::fromString(QString("1995-06-25"), QString("yyyy-MM-dd")));
    ui->calendarWidget->setMaximumDate(QDate::currentDate());

    downloadDate = QDate::currentDate().toString(QString("yyyy-MM-dd"));

    connect(ui->calendarWidget, SIGNAL (selectionChanged()), this, SLOT (onCalendarWidgetChanged()));

    connect(ui->btnPicture, SIGNAL (released()),this, SLOT (onBtnPictureClick()));
    connect(ui->btnToday, SIGNAL(clicked()), this, SLOT(onBtnTodayClick()));
    connect(ui->btnPrevious, SIGNAL(clicked()), this, SLOT(onBtnPreviousClick()));
    connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(onBtnNextClick()));
    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(onBtnCloseClick()));

    connect(&textDownloader, SIGNAL (fileDownloadCompleted()),this, SLOT (onTextDownloadComplete()));
    connect(&imageDownloader, SIGNAL (fileDownloadCompleted()),this, SLOT (onImageDownloadComplete()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCalendarWidgetChanged()
{
    qDebug() << "MainWindow::on_CalendarWidget_changed()";
    ui->calendarWidget->setEnabled(false);
    loadTextAndImage();
}

void MainWindow::onBtnPictureClick()
{
    qDebug() << "void MainWindow::handleButton()";
    loadTextAndImage();
}

void MainWindow::onBtnTodayClick()
{
    qDebug() << "void MainWindow::on_downloadButton_clicked()";
    downloadDate = QDate::currentDate().toString(QString("yyyy-MM-dd"));
    loadTextAndImage();
}

void MainWindow::onBtnPreviousClick()
{
    qDebug() << "void MainWindow::on_btnPrevious_clicked()";
    QDate myDate = ui->calendarWidget->selectedDate();
    QDate dd = myDate.addDays(-1);
    ui->calendarWidget->setSelectedDate(dd);
    ui->calendarWidget->repaint();
}

void MainWindow::onBtnNextClick()
{
    qDebug() << "void MainWindow::on_btnNext_clicked()";
    QDate myDate = ui->calendarWidget->selectedDate();
    QDate dd = myDate.addDays(1);
    ui->calendarWidget->setSelectedDate(dd);
    ui->calendarWidget->repaint();
}

void MainWindow::onBtnCloseClick()
{

}

void MainWindow::loadTextAndImage()
{
    qDebug() << "void MainWindow::loadTextAndImage()";

    // if we don't have image files saved then we need to download them
    bool needDownload = true;

    // get the date of the image we want to view
    downloadDate = ui->calendarWidget->selectedDate().toString(QString("yyyy-MM-dd"));
    qDebug() << "dateEdit =>" << downloadDate;

    // using that date construct the text and image file names
    textFileName = QDir::tempPath();
    textFileName += QString("/apod");
    textFileName += downloadDate;
    textFileName += QString(".txt");

    QFileInfo info(textFileName);
    if(info.exists()) {
        // read the file and get the image url
        QJsonObject jsonObj;
        getJson(jsonObj);

        imageUrl = jsonObj["url"].toString();

        qDebug() << imageUrl;

        getImageFileName(imageFileName);

        QFileInfo infoImg(imageFileName);
        if(infoImg.exists()) {
            loadTextFields(jsonObj);
            loadPictureBox();
            needDownload = false;
            ui->calendarWidget->setEnabled(true);
        }
    }

    if(needDownload){
        textUrl = "https://api.nasa.gov/planetary/apod?api_key=F9WlCrPBArF5YuWIKJQ2sfDM9Ko6m6dJYlsTsbsh&date=";
        textUrl += downloadDate;
        textDownloader.setUrl(textUrl);
        textDownloader.setFileName(textFileName);
        textDownloader.doDownload();
    }
}

void MainWindow::onTextDownloadComplete()
{
    QJsonObject jsonObj;
    getJson(jsonObj);

    imageUrl = jsonObj["url"].toString();

    qDebug() << imageUrl;

    loadTextFields(jsonObj);

    getImageFileName(imageFileName);

    imageDownloader.setUrl(imageUrl);
    imageDownloader.setFileName(imageFileName);
    imageDownloader.doDownload();
}

void MainWindow::onImageDownloadComplete()
{
    loadPictureBox();
    ui->calendarWidget->setEnabled(true);
}

void MainWindow::getJson(QJsonObject& obj)
{
    QFile file(textFileName);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray sFileContents;
    while (!file.atEnd()) {
        sFileContents = file.readAll();
    }

    qDebug() << "File Contents => " << sFileContents;

    QJsonDocument jsonResponse = QJsonDocument::fromJson(sFileContents);
    obj = jsonResponse.object();
}


void MainWindow::getImageFileName(QString& imgFileName)
{
    QString fileExt = imageUrl.right(4);
    imgFileName = QDir::tempPath() + QString("/apod") + downloadDate + fileExt;
}

void MainWindow::loadTextFields(const QJsonObject& obj)
{
    ui->textCopyright->setText(obj["copyright"].toString());
    ui->textDate->setText(obj["date"].toString());
    ui->textExplanation->setText(obj["explanation"].toString());
    ui->textTitle->setText(obj["title"].toString());
    ui->textServiceVersion->setText(obj["service_version"].toString());
    ui->textMediaType->setText(obj["media_type"].toString());
    ui->textUrl->setText(obj["url"].toString());
    ui->textHDUrl->setText(obj["hdurl"].toString());
}

void MainWindow::loadPictureBox()
{
    QPixmap pm;
    pm.load(imageFileName);
    ui->pictureBox->setPixmap(pm);
}

