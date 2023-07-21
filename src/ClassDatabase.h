#ifndef CLASSDATABASE_H
#define CLASSDATABASE_H

#include <QObject>
#include <QtSql>

/*
 * This class implements database object and operations on the database, in particular, SQLite
 */
class ClassDatabase : public QObject
{
    Q_OBJECT
public:
    ClassDatabase(QObject *parent);
    bool init(QString dbName);

private:
    QSqlDatabase m_db;
};

#endif // CLASSDATABASE_H
