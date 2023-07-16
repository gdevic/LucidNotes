#ifndef WIDGETTEXTEDIT_H
#define WIDGETTEXTEDIT_H

#include <QWidget>
#include <QTextDocument>

namespace Ui {
class WidgetTextEdit;
}

class WidgetTextEdit : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTextEdit(QWidget *parent = nullptr);
    ~WidgetTextEdit();

    Q_PROPERTY(bool editingToolbar READ isToolbarVisible WRITE showToolbar NOTIFY editingToolbarChanged);
    Q_PROPERTY(QTextDocument* doc READ getDoc WRITE setDoc NOTIFY docChanged);

public slots:
    bool isToolbarVisible();
    void showToolbar(bool shown);
    QTextDocument *getDoc();
    void setDoc(QTextDocument *doc);

signals:
    void editingToolbarChanged(bool);
    void docChanged(QTextDocument*);

private:
    Ui::WidgetTextEdit *ui;
};

#endif // WIDGETTEXTEDIT_H
