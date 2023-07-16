#ifndef WIDGETTEXTEDIT_H
#define WIDGETTEXTEDIT_H

#include <QWidget>

namespace Ui {
class WidgetTextEdit;
}

class WidgetTextEdit : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTextEdit(QWidget *parent = nullptr);
    ~WidgetTextEdit();

    Q_PROPERTY(bool editingToolbar MEMBER editingToolbar WRITE setEditingToolbar NOTIFY editingToolbarChanged);

public slots:
    void setEditingToolbar(bool shown);

signals:
    void editingToolbarChanged(bool);

private:
    Ui::WidgetTextEdit *ui;
    bool editingToolbar; // Show or hide editing toolbar
};

#endif // WIDGETTEXTEDIT_H
