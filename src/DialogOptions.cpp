#include "DialogOptions.h"
#include "ui_DialogOptions.h"
#include <QSettings>

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
    int index = settings.value("optionsIndex", 0).toInt();
    ui->optionTree->setCurrentItem(items[index]);
    ui->stackedWidget->setCurrentIndex(index);

    // Populate options with the values from settings or with defaults

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
    settings.setValue("optionsIndex", index);
}
