#include "WidgetTreeView.h"
#include "ui_WidgetTreeView.h"

WidgetTreeView::WidgetTreeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTreeView)
{
    ui->setupUi(this);

    ui->tbShortcuts->init(true, false, true, "Click to show shortcuts", "", "Click to find shortcut");
    ui->tbAllNotes->init(false, false, false, "Click to show all notes", "", "");
    ui->tbNotebooks->init(true, true, true, "Click to show all notebooks", "Click to add notebook", "Click to find notebook");
    ui->tbShared->init(false, false, false, "Click to show shared notes and notebooks", "", "");
    ui->tbTags->init(true, true, true, "Click to show tags view", "Click to add tag", "Click to find tag");
    ui->tbTrash->init(false, false, false, "Click to show deleted notes", "", "");


    // Test only
    static QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath("C:");
    ui->treeView->setModel(model);

    static QFileSystemModel *model2 = new QFileSystemModel;
    model2->setRootPath("D:");
    ui->treeView_2->setModel(model2);


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


// TEST:

void WidgetTreeView::on_btNewNote_clicked()
{
    qInfo() << "on_btNewNote_clicked";
}

void WidgetTreeView::on_pushButton_clicked()
{
    if (ui->treeView->isVisible())
        ui->treeView->hide();
    else
        ui->treeView->show();
}

void WidgetTreeView::on_pushButton_2_clicked()
{
    if (ui->treeView_2->isVisible())
        ui->treeView_2->hide();
    else
        ui->treeView_2->show();
}
