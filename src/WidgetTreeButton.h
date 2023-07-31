#pragma once

#include <QPushButton>

namespace Ui { class WidgetTreeButton; }

class WidgetTreeButton : public QPushButton
{
    Q_OBJECT

public:
    explicit WidgetTreeButton(QWidget *parent = nullptr);
    ~WidgetTreeButton();
    void init(bool hasTree, bool hasAdd, bool hasSearch, const QString tip, const QString tipAdd, const QString tipSearch);

    void setText(const QString text);
    void setIcon(const QIcon &icon);

    virtual void mouseDoubleClickEvent(QMouseEvent *) override;
    virtual void enterEvent(QEnterEvent *) override;
    virtual void leaveEvent(QEvent *) override;

signals:
    void expandClicked(bool);
    void plusClicked(bool);
    void searchClicked(bool);

private:
    Ui::WidgetTreeButton *ui;
    bool m_useAdd;
    bool m_useSearch;
};
