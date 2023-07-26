#pragma once

#include <QWidget>

namespace Ui { class WidgetTreeView; }

class WidgetTreeView : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTreeView(QWidget *parent = nullptr);
    ~WidgetTreeView();

private:
    Ui::WidgetTreeView *ui;
};
