#ifndef CLASSUTILS_H
#define CLASSUTILS_H

#include <QObject>
#include <QUuid>

class CUtils : public QObject
{
    Q_OBJECT

public:
    explicit CUtils(QObject *parent = nullptr);

    static QString getUUID() { return QUuid::createUuid().toString(QUuid::Id128).toUpper(); }
    static QString getUUID8() { return getUUID().left(8) + getUUID().right(8); }

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
