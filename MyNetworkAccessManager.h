#include <QtCore>
#include <QtWidgets>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QProgressDialog>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

class MyNetworkAccessManager:public QNetworkAccessManager
{
  Q_OBJECT
public:
  MyNetworkAccessManager()
  {
    pictureDate = QDate::currentDate().toString(QString("yyyy-MM-dd");
  }
  ~MyNetworkAccessManager()
  {
    delete m_pBuffer;
    delete reply;
  }
  GetPicture(QString picDate)
  {
    pictureDate = picDate;
    m_pBuffer = new QByteArray();
    QUrl url = QUrl("http://upload.wikimedia.org/wikipedia/commons/f/fe/Google_Images_Logo.png");
    QNetworkRequest req(url);
    reply = get(req);

    connect(reply,SIGNAL(readyRead()),this,SLOT(slotReadData()));
    connect(reply,SIGNAL(finished()), this,SLOT(slotFinished()));
  }

private slots:

  //If the reply is finished, save data in buffer to disk
  //as a png image: google_image_logo.png
  void slotFinished()
  {
    QString fileName = QString("apod") + pictureDate;
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
      file.write(*m_pBuffer);
      file.close();
      QMessageBox::information(NULL,"Hello!","File has been saved!");
    }
    else
    {
      QMessageBox::critical(NULL,"Hello!","Error saving file!");
    }
  }

  void slotReadData()
  {
    //append data to QByteArray buffer
    *m_pBuffer += reply->readAll();
  }

private:
  QByteArray* m_pBuffer;
  QNetworkReply* reply;
  QString pictureDate;
};
