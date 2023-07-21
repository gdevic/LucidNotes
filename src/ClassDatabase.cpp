#include "ClassDatabase.h"

ClassDatabase::ClassDatabase(QObject *parent)
    : QObject{parent}
    , m_db(QSqlDatabase::addDatabase("QSQLITE"))
{
}

/*
 * Creates and/or initializes the database file
 */
bool ClassDatabase::init(QString dbName)
{
    qInfo() << "Available database drivers:" << QSqlDatabase::drivers();
    if (!QSqlDatabase::isDriverAvailable("QSQLITE"))
        return false;

    m_db.setDatabaseName(dbName);

    if (!m_db.open())
        return false;

    // Test
    QSqlQuery query(m_db);
    query.exec("create table person (id int primary key, name varchar(20), address varchar(200), typeid int)");

    return true;
}
