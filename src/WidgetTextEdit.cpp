#include "WidgetTextEdit.h"
#include "ui_WidgetTextEdit.h"
#include <QFileDialog>
#include <QMimeData>
#include <QMimeDatabase>
#include <QStringDecoder>

WidgetTextEdit::WidgetTextEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTextEdit)
{
    ui->setupUi(this);

    // Testing:
    //   * Show or hide editing toolbar
    //   * Load a document file to be edited
    connect(ui->pbShowHide, &QPushButton::clicked, this, [=](bool shown) { showToolbar(shown); });
    connect(ui->pbLoad, &QPushButton::clicked, this, [=]() {
        QFileDialog fileDialog(this, tr("Open Document..."));
        fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
        fileDialog.setFileMode(QFileDialog::ExistingFile);
        fileDialog.setMimeTypeFilters({"text/html", "text/markdown", "text/plain"});
        if (fileDialog.exec() != QDialog::Accepted)
            return;
        const QString f = fileDialog.selectedFiles().constFirst();
        if (!QFile::exists(f))
            return;
        QFile file(f);
        if (!file.open(QFile::ReadOnly))
            return;
        QByteArray data = file.readAll();
        QMimeDatabase db;
        const QString &mimeTypeName = db.mimeTypeForFileNameAndData(f, data).name();
        if (mimeTypeName == u"text/html")
        {
            auto encoding = QStringDecoder::encodingForHtml(data);
            QString str = QStringDecoder(encoding ? *encoding : QStringDecoder::Utf8)(data);
            QUrl fileUrl = f.startsWith(u':') ? QUrl(f) : QUrl::fromLocalFile(f);
            ui->textEdit->document()->setBaseUrl(fileUrl.adjusted(QUrl::RemoveFilename));
            ui->textEdit->setHtml(str);
        }
        else if (mimeTypeName == u"text/markdown")
            ui->textEdit->setMarkdown(QString::fromUtf8(data));
        else
            ui->textEdit->setPlainText(QString::fromUtf8(data));
    });
}

WidgetTextEdit::~WidgetTextEdit()
{
    delete ui;
}

bool WidgetTextEdit::isToolbarVisible()
{
    return ui->editingToolbar->isVisible();
}

void WidgetTextEdit::showToolbar(bool shown)
{
    ui->editingToolbar->setVisible(shown);
    emit editingToolbarChanged(shown);
}

QTextDocument *WidgetTextEdit::getDoc()
{
    return ui->textEdit->document();
}

void WidgetTextEdit::setDoc(QTextDocument *doc)
{
    ui->textEdit->setDocument(doc);
    emit docChanged(doc);
}
