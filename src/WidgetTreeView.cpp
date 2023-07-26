#include "WidgetTreeView.h"
#include "ui_WidgetTreeView.h"

WidgetTreeView::WidgetTreeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTreeView)
{
    ui->setupUi(this);
}

WidgetTreeView::~WidgetTreeView()
{
    delete ui;
}
