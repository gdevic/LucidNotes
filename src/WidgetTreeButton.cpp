#include "WidgetTreeButton.h"
#include "ui_WidgetTreeButton.h"

WidgetTreeButton::WidgetTreeButton(QWidget *parent) :
    QPushButton(parent),
    ui(new Ui::WidgetTreeButton)
{
    ui->setupUi(this);

    ui->pbPlus->hide();
    ui->pbSearch->hide();

    connect(ui->pbArrow, SIGNAL(clicked(bool)), this, SIGNAL(expandClicked(bool)));
    connect(ui->pbPlus, SIGNAL(clicked(bool)), this, SIGNAL(plusClicked(bool)));
    connect(ui->pbSearch, SIGNAL(clicked(bool)), this, SIGNAL(searchClicked(bool)));
}

WidgetTreeButton::~WidgetTreeButton()
{
    delete ui;
}

void WidgetTreeButton::init(bool hasTree, bool hasAdd, bool hasSearch, const QString tip, const QString tipAdd, const QString tipSearch)
{
    ui->pbArrow->setDisabled(!hasTree);
    m_useAdd= hasAdd;
    m_useSearch = hasSearch;
    setToolTip(tip);
    ui->pbPlus->setToolTip(tipAdd);
    ui->pbSearch->setToolTip(tipSearch);
}

/*
 * Sets the button label text
 */
void WidgetTreeButton::setText(const QString text)
{
    ui->labelText->setText(text);
}

void WidgetTreeButton::setIcon(const QIcon &icon)
{
    QPixmap pix = icon.pixmap(24);
    ui->labelMainIcon->setPixmap(pix);
}

/*
 * Double-clicking the widget toggles the tree expansion.
 */
void WidgetTreeButton::mouseDoubleClickEvent(QMouseEvent *)
{
    ui->pbArrow->click();
}

/*
 * Mouse entered the widget: show the plus and search icons
 */
void WidgetTreeButton::enterEvent(QEnterEvent *)
{
    if (m_useAdd)
        ui->pbPlus->show();
    if (m_useSearch)
        ui->pbSearch->show();
}

/*
 * Mouse left the widget: hide the plus and search icons
 */
void WidgetTreeButton::leaveEvent(QEvent *)
{
    ui->pbPlus->hide();
    ui->pbSearch->hide();
}
