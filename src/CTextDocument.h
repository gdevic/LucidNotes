#pragma once

#include <QTextDocument>

class CTextDocument : public QTextDocument
{
    Q_OBJECT
public:
    explicit CTextDocument(QObject *parent = nullptr);

signals:

};
