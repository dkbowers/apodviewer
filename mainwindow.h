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

private slots:
    void handleButton();

public:
    void startRequest(QUrl url);

private slots:
    void on_downloadButton_clicked();

//    void on_quitButton_clicked();

//    void on_urlEdit_returnPressed();

    // slot for readyRead() signal
    void httpReadyRead();

    // slot for finished() signal from reply
    void httpDownloadFinished();

    // slot for downloadProgress()
    void updateDownloadProgress(qint64, qint64);

    //void enableDownloadButton();
    //void cancelDownload();

private:
    QString fileName;
    QUrl url;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *file;
    bool httpRequestAborted;
    qint64 fileSize;
    QTemporaryDir tempDir;
};

#endif // MAINWINDOW_H
