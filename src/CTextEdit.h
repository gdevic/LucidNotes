#pragma once

#include <QTextEdit>

class CTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit CTextEdit(QWidget *parent = nullptr);
};
