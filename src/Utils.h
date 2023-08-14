#pragma once

#include <QUuid>

static inline const QString getUserName() { return qEnvironmentVariable("USERNAME", qEnvironmentVariable("USER", "user")); }
static inline const QString getUUID() { return QUuid::createUuid().toString(QUuid::Id128).toUpper(); }
static inline const QString getUUID8() { return getUUID().left(8) + getUUID().right(8); }
static inline quint64 uid8toInt64(QString uid8)
{
    bool bStatus = false;
    quint64 n = uid8.toULongLong(&bStatus, 16);
    return bStatus ? n : 0;
}
typedef std::vector<unsigned char> ByteArray;
static inline ByteArray str2ba(QString s)
{
    ByteArray ba;
    QByteArray raw(s.toUtf8());
    for (int i = 0; i < raw.size(); i++)
        ba.push_back(raw.at(i));
    return ba;
}

enum LIST_BY {
    ALL,
    STACK,
    NOTE
};

#include <QWidget>

class Cursor
{
public:
    Cursor(QWidget *w, const QCursor &c) : w{w} { w->setCursor(c); }
    ~Cursor() { w->unsetCursor(); }
private:
    QWidget *w;
};
