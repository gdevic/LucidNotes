#pragma once

#include <QXmlStreamReader>

class ClassNote;

class ClassEnex : public QObject
{
    Q_OBJECT
public:
    explicit ClassEnex(QObject *parent = nullptr);
    ~ClassEnex();

    const QString import(const QString fileName);
    void cancel() { is_cancelled = true; }

    QList<ClassNote *> m_notes;
    QThread *thID {nullptr};

private:
    const QString tokenType(QXmlStreamReader &xml);
    bool readExport(QXmlStreamReader &xml);
    bool is_cancelled {false};
};
