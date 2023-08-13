#include "ClassDatabase.h"
#include <QMessageBox>

ClassDatabase::ClassDatabase(QObject *parent)
    : QObject{parent}
{
}

ClassDatabase::~ClassDatabase()
{
    if (QSqlDatabase::contains(m_connectionName))
    {
        QSqlDatabase db = QSqlDatabase::database(m_connectionName);
        db.close();
    }
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
        m_db.setConnectOptions("QSQLITE_BUSY_TIMEOUT=0");

        if (m_db.open())
        {
            m_db.exec("PRAGMA synchronous = OFF");
            m_db.exec("PRAGMA journal_mode = MEMORY");

            return QString();
        }
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
    //qInfo() << query.isSelect() << query.isActive() << query.isValid() << query.numRowsAffected() << query.size() << query.lastError().text();

    // For SELECT statements, returns true if there was a match, false otherwise
    if (query.isSelect())
        return query.first() && query.isActive();
    // For INSERT, returns the row number of the inserted record
    return query.lastInsertId().toInt();
}

/*
 * Returns the note uid or 0 if the note with that name and creation date does not exist
 * Note names are *not* unique, so an additional designator (note's creation date) is also used.
 */
uint ClassDatabase::getNoteId(const QString name, QDateTime &dateCreated)
{
    static const QString command = "SELECT uid FROM note_attr WHERE (title = ? AND date_created = ?)";

    QSqlQuery query(getDB());
    query.prepare(command);
    query.bindValue(0, name);
    query.bindValue(1, dateCreated.toString(Qt::ISODate));
    query.exec();
    //qInfo() << query.isSelect() << query.isActive() << query.isValid() << query.numRowsAffected() << query.first() << query.size() << query.lastError().text();
    if (query.lastError().isValid())
    {
        qWarning() << "queryExec error:" << query.lastError().text();
        return 0;
    }
    return query.next() ? query.value(0).toUInt() : 0;
}

/*
 * Returns the notebook uid or 0 if the notebook with that name does not exist
 * Note: notebook names are unique!
 */
uint ClassDatabase::getNotebookId(const QString name)
{
    static const QString command = "SELECT uid FROM notebook_attr WHERE (name = ?)";

    QSqlQuery query(getDB());
    query.prepare(command);
    query.bindValue(0, name);
    query.exec();
    //qInfo() << query.isSelect() << query.isActive() << query.isValid() << query.numRowsAffected() << query.first() << query.size() << query.lastError().text();
    if (query.lastError().isValid())
    {
        qWarning() << "queryExec error:" << query.lastError().text();
        return 0;
    }
    return query.next() ? query.value(0).toUInt() : 0;
}

/*
 * Creates a notebook and stack row; returns uid of the new row and true on success
 */
bool ClassDatabase::createNotebookAndStack(QString name, QString stack, uint &uid)
{
    static const QString command = "INSERT INTO notebook_attr (name, stack) VALUES (?,?)";

    QSqlQuery query(getDB());
    query.prepare(command);
    query.bindValue(0, name);
    query.bindValue(1, stack);
    query.exec();
    //qInfo() << query.isSelect() << query.isActive() << query.isValid() << query.numRowsAffected() << query.first() << query.size() << query.lastError().text();
    if (query.lastError().isValid())
    {
        qWarning() << "queryExec error:" << query.lastError().text();
        return false;
    }
    uid = query.lastInsertId().toInt();
    return true;
}

/*
 * Creates a unique notebook based on the given base name; returns uid of the new row and true on success
 * This function is used when importing notes that do not have any other notebook home
 */
bool ClassDatabase::createUniqueNotebook(QString &suggestedName, uint &uid)
{
    static const QString command = "INSERT INTO notebook_attr (name, stack) VALUES (?,?)";

    // Try the suggested notebook name as given but then modify the name if it already exists
    QString baseName(suggestedName);
    QSqlQuery query(getDB());
    query.prepare(command);

    for (int n = 0; n < 100; n++)
    {
        if (n > 0) suggestedName = QString("%1.%2").arg(baseName).arg(n);

        query.bindValue(0, suggestedName);
        query.bindValue(1, "");
        query.exec();
        //qInfo() << query.isSelect() << query.isActive() << query.isValid() << query.numRowsAffected() << query.first() << query.size() << query.lastError().text();
        if (query.lastError().isValid() && query.lastError().text().contains("UNIQUE"))
            continue;
        if (query.lastError().isValid())
        {
            qWarning() << "queryExec error:" << query.lastError().text();
            return false;
        }
        uid = query.lastInsertId().toInt();
        return true;
    }
    return false;
}
