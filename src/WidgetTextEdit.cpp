#include "WidgetTextEdit.h"
#include "ui_WidgetTextEdit.h"

WidgetTextEdit::WidgetTextEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTextEdit)
{
    ui->setupUi(this);

    // Testing: show or hide editing toolbar
    connect(ui->pushButton, &QPushButton::clicked, this, [=](bool shown) { setEditingToolbar(shown); });
}

WidgetTextEdit::~WidgetTextEdit()
{
    delete ui;
}

void WidgetTextEdit::setEditingToolbar(bool shown)
{
    ui->widget->setVisible(shown);
    emit editingToolbarChanged(shown);
}
