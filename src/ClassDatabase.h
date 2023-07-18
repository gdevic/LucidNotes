#ifndef CLASSDATABASE_H
#define CLASSDATABASE_H

#include <QObject>

/*
 * This class implements database object and operations on the database, in particular, SQLite
 */
class ClassDatabase : public QObject
{
    Q_OBJECT
public:
    explicit ClassDatabase(QObject *parent = nullptr);

signals:

};

#endif // CLASSDATABASE_H
