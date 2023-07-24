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

    connect(ui->tableNotes, SIGNAL(clicked(const QModelIndex&)), this, SLOT(cellSingleClicked(const QModelIndex&)));
    connect(ui->tableNotes, SIGNAL(activated(const QModelIndex&)), this, SLOT(cellDoubleClicked(const QModelIndex&)));
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

void WidgetTableView::cellSingleClicked(const QModelIndex &index)
{
    QString guid = m_model.index(index.row(), 1).data().toString();
    qInfo() << "Single clicked" << guid;
}

void WidgetTableView::cellDoubleClicked(const QModelIndex &index)
{
    QString guid = m_model.index(index.row(), 1).data().toString();
    qInfo() << "Double clicked" << guid;
}
