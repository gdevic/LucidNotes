#ifndef CLASSNOTE_H
#define CLASSNOTE_H

#include <QObject>
#include <QTextDocument>

class ClassNote : public QTextDocument
{
    Q_OBJECT
public:
    explicit ClassNote(QObject *parent = nullptr);

signals:

};

#endif // CLASSNOTE_H
