#ifndef CLASSNOTEBOOK_H
#define CLASSNOTEBOOK_H

#include "ClassNote.h"
#include <QObject>

/*
 * This class contains a collection of notes and operations on that collection.
 */
class ClassNotebook : public QObject
{
    Q_OBJECT
public:
    explicit ClassNotebook(QObject *parent = nullptr);

signals:

private:
    QList<ClassNote *> m_notes;
};

#endif // CLASSNOTEBOOK_H
