#pragma once

#include <QtSql>

/*
 * This class implements database object and operations on a SQLite database.
 * Each instance of this class represents a separate connection.
 * The connection needs to be opened by calling "open()" and is closed
 * when this class destructs.
 */
class ClassDatabase : public QObject
{
    Q_OBJECT
public:
    explicit ClassDatabase(QObject *parent = nullptr);
    ~ClassDatabase();
    QString open(const QString connectionName, QString databaseFileName = QString());
    bool queryExec(const QStringList &commands);
    int queryExec(const QString command, const QStringList &binds);
    QString getLastSqlError() { return m_lastError; }
    QSqlDatabase getDB() { return QSqlDatabase::database(m_connectionName); }

private:
    QString m_connectionName;
    QString m_lastError;
};
