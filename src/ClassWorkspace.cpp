#include "ClassWorkspace.h"

ClassWorkspace::ClassWorkspace(QObject *parent)
    : QObject{parent}
{
}

bool ClassWorkspace::Create(const QString &path)
{
    return 0;
}

bool ClassWorkspace::Load(const QString &path)
{
    return 0;
}

bool ClassWorkspace::Save()
{
    return 0;
}

bool ClassWorkspace::CreateRSync(const QString &name)
{
    return 0;
}
