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
    connect(ui->pushButton, SIGNAL (released()),this, SLOT (on_downloadPicture_clicked()));
    connect(ui->pushButtonToday, SIGNAL(clicked()), this, SLOT(on_downloadTodaysPicture_clicked()));

    downloadDate = QDate::currentDate().toString(QString("yyyy-MM-dd"));
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
    //on_downloadTodaysPicture_clicked();

    QString sUrl = "https://api.nasa.gov/planetary/apod?api_key=F9WlCrPBArF5YuWIKJQ2sfDM9Ko6m6dJYlsTsbsh&date=";
    sUrl += downloadDate;
    url = sUrl;

    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req( QUrl(QString("https://api.nasa.gov/planetary/apod?api_key=F9WlCrPBArF5YuWIKJQ2sfDM9Ko6m6dJYlsTsbsh") ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QString strReply = (QString)reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject jsonObj = jsonResponse.object();
        qDebug() << jsonObj["url"].toString();

        QString fileExt = jsonObj["url"].toString().right(4);
        picFileName = QDir::tempPath() + QString("/apodtest") + fileExt;

        qDebug() << picFileName;

        delete reply;

        connect(&mgr, SIGNAL(finished(QNetworkReply*)),
                    this, SLOT(slotFinished(QNetworkReply*)));

        //connect(&mgr,SIGNAL(readyRead()),this,SLOT(slotReadData()));
        //connect(&mgr,SIGNAL(finished()), this,SLOT(slotFinished()));

        QNetworkRequest req( QUrl(jsonObj["url"].toString()));
        QNetworkReply *reply1 = mgr.get(req);
        //eventLoop.exec();

/*        if (reply->error() == QNetworkReply::NoError) {
            QByteArray strReply = (QByteArray)reply1->readAll();

            QFile file(picFileName);
            if(file.open(QIODevice::WriteOnly))
            {
              file.write(strReply);
              file.close();
              QMessageBox::information(NULL,"Hello!","File has been saved!");
            }
            else
            {
              QMessageBox::critical(NULL,"Hello!","Error saving file!");
            }

            delete reply1;
        } */
    }
    else {
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
    }


}

void MainWindow::on_downloadTodaysPicture_clicked()
{
    qDebug() << "void MainWindow::on_downloadButton_clicked()";

    manager = new QNetworkAccessManager(this);

    tempDir.setPath(QDir::tempPath());


    QString sUrl = "https://api.nasa.gov/planetary/apod?api_key=F9WlCrPBArF5YuWIKJQ2sfDM9Ko6m6dJYlsTsbsh&date=";
    sUrl += downloadDate;
    url = sUrl;
    qDebug() << "url => " << sUrl;

    fileName = tempDir.path();
    fileName += QString("/apod");
    fileName += downloadDate;
    imageName = fileName;
    fileName += QString(".txt");

    if (QFile::exists(fileName)) {
        if (QMessageBox::question(this, tr("HTTP"),
                tr("There already exists a file called %1 in "
                "the current directory. Overwrite?").arg(fileName),
                QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
                == QMessageBox::No)
                return;
        QFile::remove(fileName);
    }

    file = new QFile(fileName);
    if (!file->open(QIODevice::ReadWrite)) {
        QMessageBox::information(this, tr("HTTP"),
                      tr("Unable to save the file %1: %2.")
                      .arg(fileName).arg(file->errorString()));
        delete file;
        file = 0;
        return;
    }

    // used for progressDialog
    // This will be set true when canceled from progress dialog
    httpRequestAborted = false;

    //progressDialog->setWindowTitle(tr("HTTP"));
    //progressDialog->setLabelText(tr("Downloading %1.").arg(fileName));

    startRequest(url);
}

void MainWindow::httpReadyRead()
{
    qDebug() << "void MainWindow::httpReadyRead()";

    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    QByteArray buffer = reply->readAll();
    downloadBuffer += buffer;
    if (file)
        file->write(buffer);
}

void MainWindow::updateDownloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    qDebug() << "void MainWindow::updateDownloadProgress(qint64 bytesRead, qint64 totalBytes)";

    if (httpRequestAborted)
        return;

    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesRead);
}

/* void MainWindow::on_quitButton_clicked()
{
    this->close();
}


// During the download progress, it can be canceled
void MainWindow::cancelDownload()
{
    statusLabel->setText(tr("Download canceled."));
    httpRequestAborted = true;
    reply->abort();
    downloadButton->setEnabled(true);
}  */

// When download finished or canceled, this will be called
void MainWindow::httpDownloadFinished()
{
    qDebug() << "void MainWindow::httpDownloadFinished()";

    // when canceled
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        //progressDialog->hide();
        return;
    }

    // download finished normally
    //progressDialog->hide();
    file->flush();

    //file->close();

    // get redirection url
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        file->remove();
        qDebug() << "Error => Download failed. " << reply->errorString();
    } else if (!redirectionTarget.isNull()) {
        file->close();
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        qDebug() << "Received redirect target. Redirecting to " << newUrl.toString();
        url = newUrl;
        reply->deleteLater();
        file->open(QIODevice::WriteOnly);
        file->resize(0);
        startRequest(url);

        return;
    } else {
        qDebug() << "Download success.";
        //QString fileName = QFileInfo(QUrl(urlEdit->text()).path()).fileName();
        //statusLabel->setText(tr("Downloaded %1 to %2.").arg(fileName).arg(QDir::currentPath()));
        //downloadButton->setEnabled(true);
        QByteArray jsonBytes = downloadBuffer;
        QJsonDocument doc = QJsonDocument::fromJson(jsonBytes);
        QJsonObject obj = doc.object();

        QString copyright = obj["copyright"].toString();
        qDebug() << "copyright =>" << copyright;
        QString date = obj["date"].toString();
        qDebug() << "date =>" << date;
        QString explanation = obj["explanation"].toString();
        qDebug() << "explanation =>" << explanation;
        QString hdurl = obj["hdurl"].toString();
        qDebug() << "hdurl =>" << hdurl;
        QString media_type = obj["media_type"].toString();
        qDebug() << "media_type =>" << media_type;
        QString service_version = obj["service_version"].toString();
        qDebug() << "service_version =>" << service_version;
        QString title = obj["title"].toString();
        qDebug() << "title =>" << title;
        QString url = obj["url"].toString();
        qDebug() << "url =>" << url;

        QString fileExt = url.right(4);
        imageName += fileExt;

        qDebug() << "Image Url: " << url;
        qDebug() << "Image Name: " << imageName;

        picUrl = url;
        picFileName = imageName;

        //GetFileNAM gfn(url, imageName);
        startRequest(QUrl(url));

        //DbManager dbm("apod.db");
        //dbm.addPicture(copyright, date, explanation, hdurl, media_type, service_version, title, url);


        ui->textJson->setText(url);

    }

    reply->deleteLater();
    reply = 0;
    file->close();
    delete file;
    file = 0;
    manager = 0;
}

// This will be called when download button is clicked
void MainWindow::startRequest(QUrl url)
{
    if(url.toDisplayString().left(20) == QString("https://api.nasa.gov")) {
        qDebug() << "void MainWindow::startRequest(QUrl url)";

        // get() method posts a request
        // to obtain the contents of the target request
        // and returns a new QNetworkReply object
        // opened for reading which emits
        // the readyRead() signal whenever new data arrives.
        reply = manager->get(QNetworkRequest(url));

        // Whenever more data is received from the network,
        // this readyRead() signal is emitted
        connect(reply, SIGNAL(readyRead()),
                this, SLOT(httpReadyRead()));

        // Also, downloadProgress() signal is emitted when data is received
        connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
                this, SLOT(updateDownloadProgress(qint64,qint64)));

        // This signal is emitted when the reply has finished processing.
        // After this signal is emitted,
        // there will be no more updates to the reply's data or metadata.
        connect(reply, SIGNAL(finished()),
                this, SLOT(httpDownloadFinished()));
    }
    else {
        qDebug() << "GetFileName url => " << picUrl;
        qDebug() << "GetFileName fileName => " << picFileName;

        m_pBuffer = new QByteArray();

        QUrl url(picUrl);
        reply = manager->get(QNetworkRequest(url));

        connect(reply,SIGNAL(readyRead()),this,SLOT(slotReadData()));
        connect(reply,SIGNAL(finished()), this,SLOT(slotFinished()));
    }
}

//If the reply is finished, save data in buffer to disk
//as a png image: google_image_logo.png
void MainWindow::slotFinished(QNetworkReply *reply)
{
  qDebug() << "GetFileName => slotFinished";

  QFile file(picFileName);
  if(file.open(QIODevice::WriteOnly))
  {
    //file.write(*m_pBuffer);
    file.close();
    QMessageBox::information(NULL,"Hello!","File has been saved!");
  }
  else
  {
    QMessageBox::critical(NULL,"Hello!","Error saving file!");
  }
}

void MainWindow::slotReadData()
{
    qDebug() << "GetFileName => slotReadData";

    //append data to QByteArray buffer
  *m_pBuffer += reply->readAll();
}
