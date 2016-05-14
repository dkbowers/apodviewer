#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QtWidgets>
#include <QWidget>
//#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
//#include <QProgressDialog>
#include <QFile>
#include <QFileInfo>
#include <QDir>
//#include <QMessageBox>
#include <QDate>
#include "downloadfile.h"

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
    void loadTextAndImage();
    void getJson(QJsonObject& obj);
    void loadTextFields(const QJsonObject& obj);
    void loadPictureBox(bool isYouTube = false);
    void getImageFileName(QString& imgFileName);

public:
    void startRequest(QUrl url);

private slots:
    void onTextDownloadComplete();
    void onImageDownloadComplete();

    void onBtnPictureClick();
    void onBtnTodayClick();
    void onBtnPreviousClick();
    void onBtnNextClick();
    void onBtnCloseClick();

    void onCalendarWidgetChanged();

private:
    QString downloadDate;
    QString textFileName;
    QString imageFileName;
    QString textUrl;
    QString imageUrl;
    DownloadFile imageDownloader;
    DownloadFile textDownloader;

};

#endif // MAINWINDOW_H
