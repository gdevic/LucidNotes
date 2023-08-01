#include "ClassDatabase.h"
#include <QMessageBox>

ClassDatabase::ClassDatabase(QObject *parent)
    : QObject{parent}
{
}

ClassDatabase::~ClassDatabase()
{
//    QSqlDatabase db = QSqlDatabase::database(m_connectionName);
//    db.close();
//    QSqlDatabase::removeDatabase(m_connectionName);

//    if (!QSqlDatabase::contains(m_connectionName))
//        return;
//    else
//    {
//        QSqlDatabase db = QSqlDatabase::database(m_connectionName);
//        db.close();
//    }
//    QSqlDatabase::removeDatabase(m_connectionName);
}

/*
 * Creates and/or initializes the database file, which can optionally be specified, and if not,
 * the function will use the default workspace app notes' database.
 * Returns an empty string on success or the error message describing the error, on error.
 * After a successful return from this call, the connection is opened and can be used for DB queries.
 */
QString ClassDatabase::open(const QString connectionName, QString databaseFileName)
{
    m_connectionName = connectionName;
    QString ret;
    qInfo() << "Available database drivers:" << QSqlDatabase::drivers();
    if (QSqlDatabase::isDriverAvailable("QSQLITE"))
    {
        QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE", connectionName);

        // The main SQLite database is always located in the current workspace directory
        QSettings settings;
        QString dbName = databaseFileName.isEmpty() ? settings.value("workspaceDir").toString() + "/notes.db"
                                                    : databaseFileName;
        qInfo() << "Database file:" << dbName;
        m_db.setDatabaseName(dbName);

        if (m_db.open())
            return QString();

        ret = m_db.lastError().driverText() + " " + m_db.lastError().databaseText();
    }
    else
        ret = "QSQLITE driver is not available!";
    return ret;
}

/*
 * This convenience function executes a list of SQL commands at once
 * Returns true on success, false on failure
 * Call getLastSqlError() to get the description of the failure
 */
bool ClassDatabase::queryExec(const QStringList &commands)
{
    m_lastError.clear();
    QSqlQuery query(QSqlDatabase::database(m_connectionName));
    for (auto &s : commands)
    {
        query.exec(s);
        if (query.lastError().isValid())
        {
            m_lastError = query.lastError().text();
            qWarning() << "queryExec error:" << m_lastError;
            return false;
        }
    }
    return true;
}

/*
 * This convenience function executes a single SQL command with one or more
 * positional parameters (strings) to bind.
 */
int ClassDatabase::queryExec(const QString command, const QStringList &binds)
{
    m_lastError.clear();
    QSqlQuery query(QSqlDatabase::database(m_connectionName));
    query.prepare(command);
    foreach (auto param, binds)
        query.addBindValue(param);
    query.exec();
    if (query.lastError().isValid())
    {
        m_lastError = query.lastError().text();
        qWarning() << "queryExec error:" << m_lastError;
        return 0;
    }
    //qInfo() << query.isSelect() << query.isActive() << query.isValid() << query.numRowsAffected() << query.size();

    // For SELECT statements, returns true if there was a match, false otherwise
    if (query.isSelect())
        return query.first() && query.isActive();
    // For INSERT, returns the row number of the inserted record
    return query.lastInsertId().toInt();
}
