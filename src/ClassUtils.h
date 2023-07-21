#ifndef CLASSUTILS_H
#define CLASSUTILS_H

#include <QObject>
#include <QUuid>

class CUtils : public QObject
{
    Q_OBJECT

public:
    explicit CUtils(QObject *parent = nullptr);

    static const QString getUserName() { return qEnvironmentVariable("USERNAME", qEnvironmentVariable("USER", "user")); }
    static const QString getUUID() { return QUuid::createUuid().toString(QUuid::Id128).toUpper(); }
    static const QString getUUID8() { return getUUID().left(8) + getUUID().right(8); }
    static quint64 uid8toInt64(QString uid8)
    {
        bool bStatus = false;
        quint64 n = uid8.toULongLong(&bStatus, 16);
        return bStatus ? n : 0;
    }

    typedef std::vector<unsigned char> ByteArray;
    static ByteArray str2ba(QString s)
    {
        ByteArray ba;
        QByteArray raw(s.toUtf8());
        for (int i = 0; i < raw.size(); i++)
            ba.push_back(raw.at(i));
        return ba;
    }
};

#endif // CLASSUTILS_H
