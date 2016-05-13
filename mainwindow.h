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
    void on_downloadPicture_clicked();
    void on_downloadTodaysPicture_clicked();

    // slot for readyRead() signal
    void httpReadyRead();
    void slotReadData();

    // slot for finished() signal from reply
    void httpDownloadFinished();
    void slotFinished(QNetworkReply *reply);

    // slot for downloadProgress()
    void updateDownloadProgress(qint64, qint64);

    //void enableDownloadButton();
    //void cancelDownload();

private:
    QString fileName;
    QString imageName;
    QUrl url;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *file;
    bool httpRequestAborted;
    qint64 fileSize;
    QDir tempDir;
    QByteArray downloadBuffer;
    QString downloadDate;

    QByteArray* m_pBuffer;
    QString picUrl;
    QString picFileName;
};

#endif // MAINWINDOW_H
