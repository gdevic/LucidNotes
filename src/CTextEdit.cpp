#include "CTextEdit.h"

CTextEdit::CTextEdit(QWidget *parent)
    : QTextEdit{parent}
{
    // Add the margins to the text being edited
    setViewportMargins(20, 0, 20, 0);
}
