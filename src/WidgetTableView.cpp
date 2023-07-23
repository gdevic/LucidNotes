#include "WidgetTableView.h"
#include "ui_WidgetTableView.h"

WidgetTableView::WidgetTableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTableView),
    m_db(this)
{
    ui->setupUi(this);

    // Make the vertical spacing of the table more compact
    QFontMetrics fm(ui->tableNotes->font());
    qreal vsize = fm.height() * 1.2;
    ui->tableNotes->verticalHeader()->setMinimumSectionSize(vsize);
    ui->tableNotes->verticalHeader()->setDefaultSectionSize(vsize);
}

WidgetTableView::~WidgetTableView()
{
    delete ui;
}

bool WidgetTableView::setupModel()
{
    if (!m_db.open("table"))
        return false;

    // XXX Set any kind of query, like: "SELECT * FROM note_attr WHERE author = 'anonymous'"
    m_model.setQuery("SELECT * FROM note_attr", m_db.getDB());
    m_proxy.setSourceModel(&m_model);

    m_model.setHeaderData(1, Qt::Horizontal, "GUID"); // An Example of setting up the header column text

    ui->tableNotes->setModel(&m_proxy);

    return true;
}
