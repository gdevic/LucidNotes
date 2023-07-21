#ifndef CLASSDATABASE_H
#define CLASSDATABASE_H

#include <QObject>
#include <QtSql>

/*
 * This class implements database object and operations on a SQLite database.
 * Each instance of this class represents a separate connection.
 */
class ClassDatabase : public QObject
{
    Q_OBJECT
public:
    ClassDatabase(QObject *parent);
    ~ClassDatabase();
    bool open(QString connectionName);
    bool queryExec(const QStringList &commands);
    QString getLastSqlError() { return m_lastError; }

private:
    QString m_connectionName;
    QString m_lastError;
};

#endif // CLASSDATABASE_H
