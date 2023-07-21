#ifndef CLASSENEX_H
#define CLASSENEX_H

#include <QXmlStreamReader>

class ClassNote;

class ClassEnex : public QObject
{
    Q_OBJECT
public:
    explicit ClassEnex(QObject *parent = nullptr);

    bool import(QString fileName);
    static QString tokenType(QXmlStreamReader &xml);

private:
    bool readExport(QXmlStreamReader &xml);

private:
    QList<ClassNote *> m_notes {};
};

#endif // CLASSENEX_H
