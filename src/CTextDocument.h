#ifndef CTEXTDOCUMENT_H
#define CTEXTDOCUMENT_H

#include <QTextDocument>

class CTextDocument : public QTextDocument
{
    Q_OBJECT
public:
    explicit CTextDocument(QObject *parent = nullptr);

signals:

};

#endif // CTEXTDOCUMENT_H
