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
    m_titleEdit = ui->editTitle;
    m_noteEdit = ui->editNote;

    QSettings settings;
    showToolbar(settings.value("editingToolbar", true).toBool());

    // Populate text size combo box and set the selected font and font size
    const QList<int> standardSizes = QFontDatabase::standardSizes();
    for (int size : standardSizes)
        ui->comboNoteSize->addItem(QString::number(size));
    QFont noteFont(settings.value("noteFont").value<QFont>());
    noteFont.setPointSize(settings.value("noteFontPointSize").toInt());
    m_noteEdit->setFont(noteFont);

    fontChanged(noteFont);

    QFont titleFont(settings.value("titleFont").value<QFont>());
    titleFont.setPointSize(settings.value("titleFontPointSize").toInt());
    m_titleEdit->setFont(titleFont);

    connect(m_noteEdit, &CTextEdit::currentCharFormatChanged, this, &WidgetTextEdit::currentCharFormatChanged);
    connect(m_noteEdit, &CTextEdit::cursorPositionChanged,    this, &WidgetTextEdit::cursorPositionChanged);
    connect(ui->comboNoteFont, &QComboBox::textActivated, this, &WidgetTextEdit::textFamily);
    connect(ui->comboNoteSize, &QComboBox::textActivated, this, &WidgetTextEdit::textSize);

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
            m_noteEdit->document()->setBaseUrl(fileUrl.adjusted(QUrl::RemoveFilename));
            m_noteEdit->setHtml(str);
        }
        else if (mimeTypeName == u"text/markdown")
            m_noteEdit->setMarkdown(QString::fromUtf8(data));
        else
            m_noteEdit->setPlainText(QString::fromUtf8(data));
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
    m_noteEdit->setDocument(doc);
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
    QTextCursor cursor = m_noteEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    m_noteEdit->mergeCurrentCharFormat(format);
    m_noteEdit->setFocus();
}

void WidgetTextEdit::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
}

void WidgetTextEdit::fontChanged(const QFont &f)
{
    ui->comboNoteFont->setCurrentIndex(ui->comboNoteFont->findText(QFontInfo(f).family()));
    ui->comboNoteSize->setCurrentIndex(ui->comboNoteSize->findText(QString::number(f.pointSize())));
}

void WidgetTextEdit::cursorPositionChanged()
{
}
