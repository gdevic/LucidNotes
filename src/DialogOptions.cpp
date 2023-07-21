#include "DialogOptions.h"
#include "ui_DialogOptions.h"
#include "ClassUtils.h"
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

    // Create a list of option panels and select (switch to) the last recently used (from the settings)
    const QList<QTreeWidgetItem *> items = {
        new QTreeWidgetItem(QStringList("General")),
        new QTreeWidgetItem(QStringList("Note")),
    };
    ui->optionTree->insertTopLevelItems(0, items);
    int index = settings.value("optionsPageIndex", 0).toInt();
    ui->optionTree->setCurrentItem(items[index]);
    ui->stackedWidget->setCurrentIndex(index);

    // Populate options with the values from the settings or with defaults

    // ---------- General  ----------

    ui->pbChangeWksDir->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton));

    ui->wksDir->setText(settings.value("workspaceDir").toString());
    // If the workspace directory change had already been armed, but the app has not restarted yet, color the new path
    if (settings.contains("workspaceDirNext"))
        ui->wksDir->setStyleSheet("color: rgb(255, 15, 15);");

    ui->editUserName->setText(settings.value("userName").toString());
    ui->editUserName->setPlaceholderText(CUtils::getUserName());
    QRegularExpression rx("^[a-zA-Z]+");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->editUserName->setValidator(validator);

    // ----------   Note   ----------

    ui->comboTitleFont->setCurrentFont(settings.value("titleFont").value<QFont>());
    ui->comboNoteFont->setCurrentFont(settings.value("noteFont").value<QFont>());

    // Populate font size combo boxes for title and note fonts
    const QList<int> standardSizes = QFontDatabase::standardSizes();
    for (auto size : standardSizes)
    {
        ui->comboTitleSize->addItem(QString::number(size));
        ui->comboNoteSize->addItem(QString::number(size));
    }

    auto i = ui->comboTitleSize->findText(settings.value("titleFontPointSize").toString());
    ui->comboTitleSize->setCurrentIndex(i);
    auto j = ui->comboNoteSize->findText(settings.value("noteFontPointSize").toString());
    ui->comboNoteSize->setCurrentIndex(j);

    connect(ui->optionTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onOptionTreeClicked(QTreeWidgetItem*,int)));
    connect(ui->okBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->okBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->pbChangeWksDir, SIGNAL(clicked()), this, SLOT(onChangeWksDirClicked()));
    connect(ui->pbResetWksDir, SIGNAL(clicked(bool)), this, SLOT(onResetWksDirClicked()));
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
    auto uname = ui->editUserName->text().trimmed();
    settings.setValue("userName", uname.isEmpty() ? ui->editUserName->placeholderText() : uname);
    settings.setValue("titleFont", ui->comboTitleFont->currentFont());
    settings.setValue("noteFont", ui->comboNoteFont->currentFont());
    settings.setValue("titleFontPointSize", ui->comboTitleSize->currentText());
    settings.setValue("noteFontPointSize", ui->comboNoteSize->currentText());
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

/*
 * Reset any changes to the local workspace directory
 */
void DialogOptions::onResetWksDirClicked()
{
    QSettings settings;
    ui->wksDir->setText(settings.value("workspaceDir").toString());
    ui->wksDir->setStyleSheet("");
}
