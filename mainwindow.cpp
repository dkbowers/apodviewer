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

    downloadDate = QDate::currentDate().toString(QString("yyyy-MM-dd"));

    connect(ui->pushButton, SIGNAL (released()),this, SLOT (on_downloadPicture_clicked()));
    connect(ui->pushButtonToday, SIGNAL(clicked()), this, SLOT(on_downloadTodaysPicture_clicked()));
    connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(on_btnPrevious_clicked()));
    connect(ui->btnPrevious, SIGNAL(clicked()), this, SLOT(on_btnNext_clicked()));
    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(on_btnClose_clicked()));

    connect(&textDownloader, SIGNAL (fileDownloadCompleted()),this, SLOT (onTextDownloadComplete()));
    connect(&imageDownloader, SIGNAL (fileDownloadCompleted()),this, SLOT (onImageDownloadComplete()));


    //imageDownloader.setUrl("http://apod.nasa.gov/apod/image/1501/VelaSNR-3_bigCedic1024.jpg");
    //imageDownloader.setFileName("/private/var/folders/p1/vfmdyfy97314ny63lzl9ygt00000gn/T/VelaSNR-3_bigCedic1024.jpg");
    //imageDownloader.doDownload();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_downloadPicture_clicked()
{
    qDebug() << "void MainWindow::handleButton()";
    downloadDate = ui->dateEdit->date().toString(QString("yyyy-MM-dd"));
    qDebug() << "dateEdit =>" << downloadDate;

    textUrl = "https://api.nasa.gov/planetary/apod?api_key=F9WlCrPBArF5YuWIKJQ2sfDM9Ko6m6dJYlsTsbsh&date=";
    textUrl += downloadDate;

    textFileName = QDir::tempPath();
    textFileName += QString("/apod");
    textFileName += downloadDate;
    textFileName += QString(".txt");

    textDownloader.setUrl(textUrl);
    textDownloader.setFileName(textFileName);
    textDownloader.doDownload();
}

void MainWindow::on_downloadTodaysPicture_clicked()
{
    qDebug() << "void MainWindow::on_downloadButton_clicked()";
    qDebug() << "dateEdit =>" << downloadDate;

    textUrl = "https://api.nasa.gov/planetary/apod?api_key=F9WlCrPBArF5YuWIKJQ2sfDM9Ko6m6dJYlsTsbsh&date=";
    textUrl += downloadDate;

    textFileName = QDir::tempPath();
    textFileName += QString("/apod");
    textFileName += downloadDate;
    textFileName += QString(".txt");

    textDownloader.setUrl(textUrl);
    textDownloader.setFileName(textFileName);
    textDownloader.doDownload();
}

void MainWindow::on_btnPrevious_clicked()
{
    qDebug() << "void MainWindow::on_btnPrevious_clicked()";
    QDate downloadDate = ui->dateEdit->date();
    ui->dateEdit->setDate(downloadDate.addDays(-1));
    on_downloadPicture_clicked();
}

void MainWindow::on_btnNext_clicked()
{
    qDebug() << "void MainWindow::on_btnNext_clicked()";
    QDate downloadDate = ui->dateEdit->date();
    ui->dateEdit->setDate(downloadDate.addDays(1));
    on_downloadPicture_clicked();
}

void MainWindow::on_btnClose_clicked()
{

}

void MainWindow::loadTextAndImage()
{
    qDebug() << "void MainWindow::loadTextAndImage()";

    // get the date of the image we want to view
    downloadDate = ui->dateEdit->date().toString(QString("yyyy-MM-dd"));
    qDebug() << "dateEdit =>" << downloadDate;

    // using that date construct the text and image file names
    textFileName = QDir::tempPath();
    textFileName += QString("/apod");
    textFileName += downloadDate;
    textFileName += QString(".txt");

    QFileInfo info(textFileName);
    if(info.exists()) {
        // read the file and get the image url

    }

    textUrl = "https://api.nasa.gov/planetary/apod?api_key=F9WlCrPBArF5YuWIKJQ2sfDM9Ko6m6dJYlsTsbsh&date=";
    textUrl += downloadDate;


    textDownloader.setUrl(textUrl);
    textDownloader.setFileName(textFileName);
    textDownloader.doDownload();
}

void MainWindow::onTextDownloadComplete()
{
    QFile file(textFileName);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray sFileContents;
    while (!file.atEnd()) {
        //QByteArray line = file.readLine();
        sFileContents = file.readAll();
    }

    qDebug() << "File Contents => " << sFileContents;

    QJsonDocument jsonResponse = QJsonDocument::fromJson(sFileContents);
    QJsonObject jsonObj = jsonResponse.object();
    imageUrl = jsonObj["url"].toString();
    qDebug() << imageUrl;

    ui->textCopyright->setText(jsonObj["copyright"].toString());
    ui->textDate->setText(jsonObj["date"].toString());
    ui->textExplanation->setText(jsonObj["explanation"].toString());
    ui->textTitle->setText(jsonObj["title"].toString());
    ui->textServiceVersion->setText(jsonObj["service_version"].toString());
    ui->textMediaType->setText(jsonObj["media_type"].toString());
    ui->textUrl->setText(jsonObj["url"].toString());
    ui->textHDUrl->setText(jsonObj["hdurl"].toString());

    QString fileExt = imageUrl.right(4);
    imageFileName = QDir::tempPath() + QString("/apod") + downloadDate + fileExt;

    imageDownloader.setUrl(imageUrl);
    imageDownloader.setFileName(imageFileName);
    imageDownloader.doDownload();
}

void MainWindow::onImageDownloadComplete()
{
    QPixmap pm;
    pm.load(imageFileName);

    ui->pictureBox->setPixmap(pm);
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

void MainWindow::getImageFileName(QString& imgFileName)
{
    QString fileExt = imageUrl.right(4);
    imgFileName = QDir::tempPath() + QString("/apod") + downloadDate + fileExt;
}
