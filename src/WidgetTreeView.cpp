#include "WidgetTreeView.h"
#include "ui_WidgetTreeView.h"

#include "ClassDatabase.h"
#include "CTreeModel.h"

WidgetTreeView::WidgetTreeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTreeView)
{
    ui->setupUi(this);

    // This connect needs to be before calling init since the init may emit a signal to expand the tree view
    connect(ui->tbNotebooks, SIGNAL(expandClicked(bool)), this, SLOT(onNotebooksExpandClicked(bool)));

    ui->tbShortcuts->init(true, false, true, "Click to show shortcuts", "", "Click to find shortcut");
    ui->tbAllNotes->init(false, false, false, "Click to show all notes", "", "");
    ui->tbNotebooks->init(true, true, true, "Click to show all notebooks", "Click to add notebook", "Click to find notebook");
    ui->tbShared->init(false, false, false, "Click to show shared notes and notebooks", "", "");
    ui->tbTags->init(true, true, true, "Click to show tags view", "Click to add tag", "Click to find tag");
    ui->tbTrash->init(false, false, false, "Click to show deleted notes", "", "");

    // Test only
//    static QFileSystemModel *model = new QFileSystemModel;
//    model->setRootPath("C:");
//    ui->treeNotebooks->setModel(model);

//    static ClassDatabase m_db;
//    static QSqlQueryModel m_model;
//    QString ret = m_db.open("tree");

//    m_model.setQuery("SELECT stack,name FROM notebook_attr ORDER BY stack", m_db.getDB());
//    ui->treeNotebooks->setModel(&m_model);


    QFile file("T:/CTreeModel.txt");
    file.open(QIODevice::ReadOnly);
    QStringList header;
    header << "Column A";
    header << "Column B";
    CTreeModel *model = new CTreeModel(header, file.readAll(), this);
    file.close();

    ui->treeNotebooks->setModel(model);


    connect(ui->tbShortcuts, SIGNAL(clicked(bool)), this, SLOT(onShortcutsClicked(bool)));
    connect(ui->tbShortcuts, SIGNAL(expandClicked(bool)), this, SLOT(onShortcutsExpandClicked(bool)));
    connect(ui->tbShortcuts, SIGNAL(plusClicked(bool)), this, SLOT(onShortcutsPlusClicked(bool)));
    connect(ui->tbShortcuts, SIGNAL(searchClicked(bool)), this, SLOT(onShortcutsSearchClicked(bool)));
}

WidgetTreeView::~WidgetTreeView()
{
    delete ui;
}

void WidgetTreeView::onShortcutsClicked(bool checked)
{
    qInfo() << "Shortcuts CLICKED!" << checked;
}

void WidgetTreeView::onShortcutsExpandClicked(bool expanded)
{
    qInfo() << "Shortcuts EXPAND" << expanded;
}

void WidgetTreeView::onShortcutsPlusClicked(bool)
{
    qInfo() << "Shortcuts PLUS";
}

void WidgetTreeView::onShortcutsSearchClicked(bool)
{
    qInfo() << "Shortcuts SEARCH";
}

void WidgetTreeView::onNotebooksExpandClicked(bool expanded)
{
    ui->treeNotebooks->setHidden(!expanded);
}

// TEST:

void WidgetTreeView::on_btNewNote_clicked()
{
    qInfo() << "on_btNewNote_clicked";
}
