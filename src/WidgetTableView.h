#ifndef WIDGETTABLEVIEW_H
#define WIDGETTABLEVIEW_H

#include <QWidget>

namespace Ui { class WidgetTableView; }

class WidgetTableView : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTableView(QWidget *parent = nullptr);
    ~WidgetTableView();

private:
    Ui::WidgetTableView *ui;
};

#endif // WIDGETTABLEVIEW_H
