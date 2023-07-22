#include "WidgetTableView.h"
#include "ui_WidgetTableView.h"

WidgetTableView::WidgetTableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTableView)
{
    ui->setupUi(this);
}

WidgetTableView::~WidgetTableView()
{
    delete ui;
}
