#include "httpdownload.h"

HttpDownload::HttpDownload(QWidget *parent) :
    QDialog(parent)
{
    QRect rect;
    rect.setLeft(0);
    rect.setTop(0);
    rect.setWidth(640);
    rect.setHeight(480);
    setGeometry(QRect(0, 0, 335, 133));

    setWindowTitle("HttpDownload");

    gridLayout = new QGridLayout(this);

    urlLabel = new QLabel(this);
    urlLabel->setObjectName("urlLabel");
    urlLabel->setText("Url: ");
    gridLayout->addWidget(urlLabel, 0, 0);

    urlEdit = new QLineEdit(this);
    gridLayout->addWidget(urlEdit, 0, 1);

    horizontalLayout_2 = new QHBoxLayout();

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Fixed);
    horizontalLayout_2->addSpacerItem(horizontalSpacer_2);

    statusLabel = new QLabel(this);
    statusLabel->setObjectName("statusLabel");
    statusLabel->setText("Type URL of a file");
    horizontalLayout_2->addWidget(statusLabel);

    horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Fixed);
    horizontalLayout_2->addSpacerItem(horizontalSpacer_3);
    gridLayout->addLayout(horizontalLayout_2, 1, 0);

    verticalSpacer = new QSpacerItem(291, 27, QSizePolicy::Fixed, QSizePolicy::Expanding);
    //gridLayout->addSpacerItem(verticalSpacer);

    horizontalLayout_3 = new QHBoxLayout();

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Fixed);
    horizontalLayout_3->addSpacerItem(horizontalSpacer);

    downloadButton = new QPushButton(this);
    downloadButton->setText("Download");
    horizontalLayout_3->addWidget(downloadButton);

    quitButton = new QPushButton(this);
    quitButton->setText("Quit");
    horizontalLayout_3->addWidget(quitButton);

    gridLayout->addLayout(horizontalLayout_3, 3, 0);

    this->setLayout(gridLayout);
    gridLayout->setMargin(11);
    gridLayout->setSpacing(6);

    urlEdit->setText("http://qt.com");
    statusLabel->setWordWrap(true);
    downloadButton->setDefault(true);
    quitButton->setAutoDefault(false);

    progressDialog = new QProgressDialog(this);

    connect(urlEdit, SIGNAL(textChanged(QString)),
                this, SLOT(enableDownloadButton()));
    connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelDownload()));
}

HttpDownload::~HttpDownload()
{

}

void HttpDownload::on_downloadButton_clicked()
{
    manager = new QNetworkAccessManager(this);

    // get url
    url = urlEdit->text();

    QFileInfo fileInfo(url.path());
    QString fileName = fileInfo.fileName();

    if (fileName.isEmpty())
        fileName = "index.html";

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
    if (!file->open(QIODevice::WriteOnly)) {
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

    progressDialog->setWindowTitle(tr("HTTP"));
    progressDialog->setLabelText(tr("Downloading %1.").arg(fileName));

    // download button disabled after requesting download
    downloadButton->setEnabled(false);

    startRequest(url);
}

void HttpDownload::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (file)
        file->write(reply->readAll());
}

void HttpDownload::updateDownloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    if (httpRequestAborted)
        return;

    progressDialog->setMaximum(totalBytes);
    progressDialog->setValue(bytesRead);
}

void HttpDownload::on_quitButton_clicked()
{
    this->close();
}

void HttpDownload::on_urlEdit_returnPressed()
{
    on_downloadButton_clicked();
}

void HttpDownload::enableDownloadButton()
{
    downloadButton->setEnabled(!(urlEdit->text()).isEmpty());
}

// During the download progress, it can be canceled
void HttpDownload::cancelDownload()
{
    statusLabel->setText(tr("Download canceled."));
    httpRequestAborted = true;
    reply->abort();
    downloadButton->setEnabled(true);
}

// When download finished or canceled, this will be called
void HttpDownload::httpDownloadFinished()
{
    // when canceled
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        progressDialog->hide();
        return;
    }

    // download finished normally
    progressDialog->hide();
    file->flush();
    file->close();

    // get redirection url
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        file->remove();
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Download failed: %1.")
                                 .arg(reply->errorString()));
        downloadButton->setEnabled(true);
    } else if (!redirectionTarget.isNull()) {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        if (QMessageBox::question(this, tr("HTTP"),
                                  tr("Redirect to %1 ?").arg(newUrl.toString()),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            url = newUrl;
            reply->deleteLater();
            file->open(QIODevice::WriteOnly);
            file->resize(0);
            startRequest(url);
            return;
        }
    } else {
        QString fileName = QFileInfo(QUrl(urlEdit->text()).path()).fileName();
        statusLabel->setText(tr("Downloaded %1 to %2.").arg(fileName).arg(QDir::currentPath()));
        downloadButton->setEnabled(true);
    }

    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;
    manager = 0;
}

// This will be called when download button is clicked
void HttpDownload::startRequest(QUrl url)
{
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
