#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QWidget>
//#include <QNetwork>
#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QProgressDialog>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QDate>
#include "downloadtext.h"
#include "downloadimage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public:
    void startRequest(QUrl url);

private slots:
    void onTextDownloadComplete();
    void onImageDownloadComplete();

    void on_downloadPicture_clicked();
    void on_downloadTodaysPicture_clicked();

private:
    QString downloadDate;
    QString textFileName;
    QString imageFileName;
    QString textUrl;
    QString imageUrl;
    DownloadImage imageDownloader;
    DownloadText textDownloader;

};

#endif // MAINWINDOW_H
