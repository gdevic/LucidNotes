#include "WidgetTreeView.h"
#include "ui_WidgetTreeView.h"
#include "ClassDatabase.h"
#include <QStandardItemModel>

struct CStandardItem: public QStandardItem
{
    CStandardItem(const QString name, bool is_stack = false) : QStandardItem(name), name(name), is_stack(is_stack) {}
    const QString name;
    const bool is_stack;
};

class CTreeModel: public QStandardItemModel
{
public:
    CTreeModel() {
    }
    ~CTreeModel() {}

    void makeTree(QStandardItem *rootItem)
    {
        ClassDatabase db;
        QString ret = db.open("treeView");
        Q_ASSERT_X(ret.isEmpty(), __FUNCTION__, ret.toStdString().c_str());
        QSqlQuery qdb(QSqlDatabase::database("treeView"));
        qdb.clear();
        qdb.prepare("SELECT stack,name FROM notebook_attr ORDER BY stack");
        qdb.exec();

        CStandardItem *curStackItem = static_cast<CStandardItem *>(rootItem);
        QString curStack = "";
        while (qdb.next())
        {
            QString stack = qdb.value(0).toString();
            QString book = qdb.value(1).toString();
            qInfo() << stack << book;

            if (stack == curStack)
            {
                CStandardItem *item = new CStandardItem(book);
                curStackItem->appendRow(item);
            }
            else
            {
                if (curStackItem->text() != stack)
                {
                    curStackItem = new CStandardItem(stack, true);
                    rootItem->appendRow(curStackItem);
                }
                CStandardItem *newBookItem = new CStandardItem(book);
                curStackItem->appendRow(newBookItem);
            }
        }
        db.getDB().close();
    }

private:
    QStandardItem *root{};
};

WidgetTreeView::WidgetTreeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTreeView),
    m_tmodel(new CTreeModel)
{
    ui->setupUi(this);

    // When the user clicks on "All Notes", issue a signal to list all notes
    connect(ui->tbAllNotes, &QPushButton::clicked, this, [this]{ emit updateNotelist(QString(),LIST_BY::ALL);});

    // This connect needs to be before calling init since the init may emit a signal to expand the tree view
    connect(ui->tbNotebooks, SIGNAL(expandClicked(bool)), this, SLOT(onNotebooksExpandClicked(bool)));

    ui->tbShortcuts->init(true, false, true, "Click to show shortcuts", "", "Click to find shortcut");
    ui->tbAllNotes->init(false, false, false, "Click to show all notes", "", "");
    ui->tbNotebooks->init(true, true, true, "Click to show all notebooks", "Click to add notebook", "Click to find notebook");
    ui->tbShared->init(false, false, false, "Click to show shared notes and notebooks", "", "");
    ui->tbTags->init(true, true, true, "Click to show tags view", "Click to add tag", "Click to find tag");
    ui->tbTrash->init(false, false, false, "Click to show deleted notes", "", "");


    m_tmodel->makeTree(m_tmodel->invisibleRootItem());
    ui->treeNotebooks->setModel(m_tmodel);

    connect(ui->treeNotebooks, &QTreeView::clicked, this, &WidgetTreeView::onNotebooksLeafClicked);

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

/*
 * User clicked on the Notebooks tree view, either on a stack item or on a leaf notebook
 */
void WidgetTreeView::onNotebooksLeafClicked(const QModelIndex &index)
{
    CStandardItem *item = static_cast<CStandardItem *>(m_tmodel->itemFromIndex(index));
    qInfo() << item->name << item->is_stack;
    emit updateNotelist(item->name, item->is_stack ? LIST_BY::STACK : LIST_BY::NOTE);
}

// TEST:

void WidgetTreeView::on_btNewNote_clicked()
{
    qInfo() << "on_btNewNote_clicked";
}

void WidgetTreeView::setupModel()
{
    // BAD code
    m_tmodel = new CTreeModel;
    m_tmodel->makeTree(m_tmodel->invisibleRootItem());
    ui->treeNotebooks->setModel(m_tmodel);
}
