#ifndef CTEXTEDIT_H
#define CTEXTEDIT_H

#include <QTextEdit>

class CTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit CTextEdit(QWidget *parent = nullptr);

signals:

};

#endif // CTEXTEDIT_H
