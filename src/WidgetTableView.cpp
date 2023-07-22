#include "WidgetTableView.h"
#include "ui_WidgetTableView.h"

WidgetTableView::WidgetTableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTableView),
    m_db(this)
{
    ui->setupUi(this);
}

WidgetTableView::~WidgetTableView()
{
    delete ui;
}

bool WidgetTableView::setupModel()
{
    if (!m_db.open("table"))
        return false;

    m_model = new QSqlTableModel(ui->tableNotes, m_db.getDB());

    m_model->setTable("note_attr");
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->select();

    ui->tableNotes->setModel(m_model);

    return true;
}
