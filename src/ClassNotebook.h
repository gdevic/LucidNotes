#ifndef CLASSNOTEBOOK_H
#define CLASSNOTEBOOK_H

#include "ClassNote.h"
#include <QObject>

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
