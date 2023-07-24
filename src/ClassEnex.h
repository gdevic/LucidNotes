#pragma once

#include <QXmlStreamReader>

class ClassNote;

class ClassEnex : public QObject
{
    Q_OBJECT
public:
    explicit ClassEnex(QObject *parent = nullptr);
    ~ClassEnex();

    bool import(QString fileName);
    QList<ClassNote *> &getNotes() { return m_notes; }

    static QString tokenType(QXmlStreamReader &xml);

private:
    bool readExport(QXmlStreamReader &xml);

private:
    QList<ClassNote *> m_notes {};
};
