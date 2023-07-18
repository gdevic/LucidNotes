#ifndef CLASSNOTE_H
#define CLASSNOTE_H

#include <QObject>

/*
 * This class contains a note text and all meta information for that note
 * as well as operations on that note
 */
class ClassNote : public QObject
{
    Q_OBJECT
public:
    explicit ClassNote(QObject *parent = nullptr);

signals:

};

#endif // CLASSNOTE_H
