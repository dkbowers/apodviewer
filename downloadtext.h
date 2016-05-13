#ifndef DOWNLOADTEXT_H
#define DOWNLOADTEXT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QFile>
#include <QDebug>

class DownloadText : public QObject
{
    Q_OBJECT
public:
    explicit DownloadText(QObject *parent = 0);
    void setUrl(QString sUrl) { urlString = sUrl; }
    void setFileName(QString sFileName) { fileNameString = sFileName; }
    void doDownload();

signals:
    void fileDownloadCompleted();

public slots:
    void replyFinished (QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QString urlString;
    QString fileNameString;
};

#endif // DOWNLOADTEXT_H
