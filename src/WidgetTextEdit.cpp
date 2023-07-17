#include "WidgetTextEdit.h"
#include "ui_WidgetTextEdit.h"
#include <QFileDialog>
#include <QMimeData>
#include <QMimeDatabase>
#include <QSettings>
#include <QStringDecoder>

WidgetTextEdit::WidgetTextEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTextEdit)
{
    ui->setupUi(this);
    m_editingToolbar = ui->editingToolbar;
    m_textEdit = ui->textEdit;

    QSettings settings;
    showToolbar(settings.value("editingToolbar", true).toBool());

    // Get the current font
    ui->comboFont->setCurrentFont(settings.value("font", QApplication::font()).value<QFont>());

    // Populate text size combo box
    const QList<int> standardSizes = QFontDatabase::standardSizes();
    for (int size : standardSizes)
        ui->comboSize->addItem(QString::number(size));
    int fontSizeIndex = settings.value("fontSizeIndex", 0).toInt();
    ui->comboSize->setCurrentIndex(fontSizeIndex);

    connect(m_textEdit, &CTextEdit::currentCharFormatChanged, this, &WidgetTextEdit::currentCharFormatChanged);
    connect(m_textEdit, &CTextEdit::cursorPositionChanged,    this, &WidgetTextEdit::cursorPositionChanged);
    connect(ui->comboFont, &QComboBox::textActivated, this, &WidgetTextEdit::textFamily);
    connect(ui->comboSize, &QComboBox::textActivated, this, &WidgetTextEdit::textSize);

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
            m_textEdit->document()->setBaseUrl(fileUrl.adjusted(QUrl::RemoveFilename));
            m_textEdit->setHtml(str);
        }
        else if (mimeTypeName == u"text/markdown")
            m_textEdit->setMarkdown(QString::fromUtf8(data));
        else
            m_textEdit->setPlainText(QString::fromUtf8(data));
    });
}

WidgetTextEdit::~WidgetTextEdit()
{
    delete ui;
}

void WidgetTextEdit::showToolbar(bool shown)
{
    m_editingToolbar->setVisible(shown);

    QSettings settings;
    settings.setValue("editingToolbar", shown);
}

void WidgetTextEdit::setDoc(QTextDocument *doc)
{
    m_textEdit->setDocument(doc);
}

void WidgetTextEdit::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamilies({f});
    mergeFormatOnWordOrSelection(fmt);
}

void WidgetTextEdit::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (pointSize > 0)
    {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void WidgetTextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = m_textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    m_textEdit->mergeCurrentCharFormat(format);
}

void WidgetTextEdit::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
}

void WidgetTextEdit::fontChanged(const QFont &f)
{
    ui->comboFont->setCurrentIndex(ui->comboFont->findText(QFontInfo(f).family()));
    ui->comboSize->setCurrentIndex(ui->comboSize->findText(QString::number(f.pointSize())));
}

void WidgetTextEdit::cursorPositionChanged()
{
}
