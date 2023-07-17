#include "DialogOptions.h"
#include "ui_DialogOptions.h"
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>

DialogOptions::DialogOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOptions)
{
    ui->setupUi(this);

    QSettings settings;
    restoreGeometry(settings.value("optionsGeometry").toByteArray());

    // Create the list of option panels and select (switch to) the last recently used (from settings)
    const QList<QTreeWidgetItem *> items = {
        new QTreeWidgetItem(QStringList("General")),
        new QTreeWidgetItem(QStringList("Note")),
    };
    ui->optionTree->insertTopLevelItems(0, items);
    int index = settings.value("optionsPageIndex", 0).toInt();
    ui->optionTree->setCurrentItem(items[index]);
    ui->stackedWidget->setCurrentIndex(index);

    // Populate options with the values from settings or with defaults

    // ---------- General  ----------

    ui->wksDir->setText(settings.value("workspaceDir").toString());
    // If the workspace directory change had already been armed, but the app has not restarted yet, color the new path
    if (!settings.value("workspaceDirNext").toString().isEmpty())
        ui->wksDir->setStyleSheet("color: rgb(255, 15, 15);");

    // ----------   Note   ----------

    // Get the current font
    ui->comboFont->setCurrentFont(settings.value("font", QApplication::font()).value<QFont>());

    // Populate text size combo box
    int defaultSize = QApplication::font().pointSize();
    const QList<int> standardSizes = QFontDatabase::standardSizes();
    for (int size : standardSizes)
        ui->comboSize->addItem(QString::number(size));
    int fontSizeIndex = settings.value("fontSizeIndex", standardSizes.indexOf(defaultSize)).toInt();
    ui->comboSize->setCurrentIndex(fontSizeIndex);

    connect(ui->optionTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onOptionTreeClicked(QTreeWidgetItem*,int)));
    connect(ui->okBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->okBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->pbChangeWksDir, SIGNAL(clicked()), this, SLOT(onChangeWksDirClicked()));
}

DialogOptions::~DialogOptions()
{
    QSettings settings;
    settings.setValue("optionsGeometry", saveGeometry());

    delete ui;
}

/*
 * Apply changed settings (we simply store back every setting)
 */
void DialogOptions::onApply()
{
    QSettings settings;
    // The workspace directory, if changed, should take effect when the app restarts
    settings.remove("workspaceDirNext");
    if (settings.value("workspaceDir").toString() != ui->wksDir->text())
        settings.setValue("workspaceDirNext", ui->wksDir->text()); // Arms the new workspace directory
    settings.setValue("font", ui->comboFont->currentFont());
    settings.setValue("fontSizeIndex", ui->comboSize->currentIndex());
}

/*
 * We are processing our own accept signal, sent when user clicks on the OK button
 */
void DialogOptions::accept()
{
    onApply();
    QDialog::done(QDialog::Accepted);
}

/*
 * In the case of reject (user clicked the Cancel button), do not save the changes
 */
void DialogOptions::reject()
{
    QDialog::done(QDialog::Rejected);
}

/*
 * Change the active page of the right page (stacked pages) based on which option was selected
 */
void DialogOptions::onOptionTreeClicked(QTreeWidgetItem *item, int)
{
    int index = ui->optionTree->indexOfTopLevelItem(item);
    ui->stackedWidget->setCurrentIndex(index);

    QSettings settings;
    settings.setValue("optionsPageIndex", index);
}

/*
 * Select the local workspace directory
 */
void DialogOptions::onChangeWksDirClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select new local directory", ui->wksDir->text(), QFileDialog::ShowDirsOnly);
    if (!dir.isEmpty() && (dir != ui->wksDir->text()))
    {
        qInfo() << "New workspace directory:" << dir;
        ui->wksDir->setText(dir);
        ui->wksDir->setStyleSheet("color: rgb(255, 15, 15);");
    }
}
