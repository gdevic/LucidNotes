#include "WidgetTableView.h"
#include "ui_WidgetTableView.h"

WidgetTableView::WidgetTableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTableView),
    m_sqlModel(this),
    m_pxProxy(this),
    m_sortProxy(this)
{
    ui->setupUi(this);

    QString ret = m_db.open("tableView");
    Q_ASSERT_X(ret.isEmpty(), __FUNCTION__, ret.toStdString().c_str());

    // Make the vertical spacing of the table more compact
    QFontMetrics fm(ui->tableNotes->font());
    qreal vsize = fm.height() * 1.2;
    ui->tableNotes->verticalHeader()->setMinimumSectionSize(vsize);
    ui->tableNotes->verticalHeader()->setDefaultSectionSize(vsize);

    QSettings settings;
    ui->tableNotes->horizontalHeader()->restoreState(settings.value("tableHeader").toByteArray());
    ui->tableNotes->horizontalHeader()->setSectionsMovable(true);

    /*
     * User single-clicked on a table cell
     */
    connect(ui->tableNotes, &QTableView::clicked, this, [=](const QModelIndex &index)
    {
        QString guid = m_sqlModel.index(m_sortProxy.mapToSource(index).row(), 1).data().toString();
        qInfo() << "Single clicked" << guid;
        emit noteSingleClicked(guid);
    });

    /*
     * User double-clicked on a table cell
     */
    connect(ui->tableNotes, &QTableView::activated, this, [=](const QModelIndex &index)
    {
        QString guid = m_sqlModel.index(m_sortProxy.mapToSource(index).row(), 1).data().toString();
        qInfo() << "Double clicked" << guid;
        emit noteDoubleClicked(guid);
    });

    // Set up the default list of notes and the chain of governing models
    onUpdateQuery("", LIST_BY::ALL);
    m_sqlModel.setHeaderData(1, Qt::Horizontal, "GUID"); // An Example of setting up the header column text

    // Chain all models: QSqlQueryModel -> PopulateAllProxy -> QSortFilterProxyModel => (QTableView)
    m_pxProxy.setSourceModel(&m_sqlModel);
    m_sortProxy.setSourceModel(&m_pxProxy);
    ui->tableNotes->setModel(&m_sortProxy);
}

WidgetTableView::~WidgetTableView()
{
    QSettings settings;
    settings.setValue("tableHeader", ui->tableNotes->horizontalHeader()->saveState());

    delete ui;
}

bool WidgetTableView::setupModel()
{
    return true;
}

/*
 * Change the query that the table view uses to list the notes
 */
void WidgetTableView::onUpdateQuery(const QString name, LIST_BY key)
{
    QSqlQuery query(m_db.getDB());
    if (key == LIST_BY::ALL)
        query.prepare("SELECT * FROM note_attr");
    if (key == LIST_BY::STACK)
    {
        query.prepare("SELECT * FROM note_attr WHERE (notebook = ?)");
        query.bindValue(0, name);
    }
    if (key == LIST_BY::NOTE)
    {
        query.prepare("SELECT * FROM note_attr WHERE (notebook = ?)");
        query.bindValue(0, name);
    }
    query.exec();
    m_sqlModel.setQuery(query);
}
