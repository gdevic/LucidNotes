#pragma once

#include <QWidget>

namespace Ui { class WidgetTextEdit; }
class CTextEdit;
class QLineEdit;
class QTextCharFormat;
class QTextDocument;

class WidgetTextEdit : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTextEdit(QWidget *parent = nullptr);
    ~WidgetTextEdit();

public slots:
    bool isToolbarVisible()           { return m_editingToolbar->isVisible(); }
    void showToolbar(bool shown);
    void setDoc(QTextDocument *doc);

private slots:
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void currentCharFormatChanged(const QTextCharFormat &format);
    void cursorPositionChanged();

private:
    Ui::WidgetTextEdit *ui;
    QWidget *m_editingToolbar;
    QLineEdit *m_titleEdit;
    CTextEdit *m_noteEdit;

    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void fontChanged(const QFont &f);
};
