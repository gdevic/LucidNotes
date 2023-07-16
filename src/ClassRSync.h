#ifndef CLASSRSYNC_H
#define CLASSRSYNC_H

/* This class contains a remote sync definition and operations on it */

#include <QObject>

class ClassRSync : public QObject
{
    Q_OBJECT
public:
    explicit ClassRSync(QObject *parent = nullptr);

signals:

};

#endif // CLASSRSYNC_H
