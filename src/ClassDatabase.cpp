#include "ClassDatabase.h"
#include <QMessageBox>

ClassDatabase::ClassDatabase(QObject *parent)
    : QObject{parent}
{
}

ClassDatabase::~ClassDatabase()
{
    if (!QSqlDatabase::contains(m_connectionName))
        return;
    else
    {
        QSqlDatabase db = QSqlDatabase::database(m_connectionName);
        db.close();
    }
    QSqlDatabase::removeDatabase(m_connectionName);
}

/*
 * Creates and/or initializes the database file
 */
bool ClassDatabase::open(QString connectionName)
{
    m_connectionName = connectionName;
    QString err;
    qInfo() << "Available database drivers:" << QSqlDatabase::drivers();
    if (QSqlDatabase::isDriverAvailable("QSQLITE"))
    {
        QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE", connectionName);

        // The main SQLite database is always located in the current workspace directory
        QSettings settings;
        QString dbName = settings.value("workspaceDir").toString() + "/notes.db";
        qInfo() << "Database file:" << dbName;
        m_db.setDatabaseName(dbName);

        if (m_db.open())
            return true;

        err = m_db.lastError().driverText() + " " + m_db.lastError().databaseText();
    }
    else
        err = "QSQLITE driver is not available!";
    QMessageBox::critical(nullptr, "Notes",
        "Unable to initialize local SQLite database:\n" + err +
        "\nPlease make sure this application is installed correctly.\n");
    return false;
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
