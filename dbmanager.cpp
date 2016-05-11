#include <dbmanager.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

DbManager::DbManager(const QString &path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open())
    {
        qDebug() << "Error: connection with database fail";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }
}

DbManager::~DbManager()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool DbManager::isOpen() const
{
    return m_db.isOpen();
}

bool DbManager::addPicture(const QString& copyrightField,
                           const QString& dateField,
                           const QString& explanationField,
                           const QString& hdurlField,
                           const QString& media_typeField,
                           const QString& service_versionField,
                           const QString& titleField,
                           const QString& url)
{
    bool success = false;

    if (!name.isEmpty())
    {
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO apod (copyright, picdate, explanation, hdurl, media_type, service_version, title, url) VALUES (:copyrightField,:dateField, :explanationField, :hdurlField, :media_typeField, :service_versionField, :titleField, :url)");
        queryAdd.bindValue(":copyrightField", copyrightField);
        queryAdd.bindValue(":dateField", dateField);
        queryAdd.bindValue(":explanationField", explanationField);
        queryAdd.bindValue(":hdurlField", hdurlField);
        queryAdd.bindValue(":media_typeField", media_typeField);
        queryAdd.bindValue(":service_versionField", service_versionField);
        queryAdd.bindValue(":titleField", titleField);
        queryAdd.bindValue(":url", url);

        if(queryAdd.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << "add person failed: " << queryAdd.lastError();
        }
    }
    else
    {
        qDebug() << "add person failed: name cannot be empty";
    }

    return success;
}


bool DbManager::removePicture(const QString& date)
{
    bool success = false;

    if (personExists(name))
    {
        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM apod WHERE picdate = (:date)");
        queryDelete.bindValue(":date", date);
        success = queryDelete.exec();

        if(!success)
        {
            qDebug() << "remove picture failed: " << queryDelete.lastError();
        }
    }
    else
    {
        qDebug() << "remove picture failed: picture doesnt exist";
    }

    return success;
}

void DbManager::printAllPictures() const
{
    qDebug() << "Pictures in db:";
    QSqlQuery query("SELECT * FROM apod");
    int idUrl = query.record().indexOf("url");
    while (query.next())
    {
        QString url = query.value(idUrl).toString();
        qDebug() << "===" << url;
    }
}

bool DbManager::pictureExists(const QString& date) const
{
    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT url FROM apod WHERE picdate = (:date)");
    checkQuery.bindValue(":date", date);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }
    else
    {
        qDebug() << "picture exists failed: " << checkQuery.lastError();
    }

    return exists;
}

bool DbManager::removeAllPictures()
{
    bool success = false;

    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM apod");

    if (removeQuery.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "remove all pictures failed: " << removeQuery.lastError();
    }

    return success;
}