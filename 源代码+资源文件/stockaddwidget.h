#ifndef STOCKADDWIDGET_H
#define STOCKADDWIDGET_H

#include <QWidget>
#include <QSqlTableModel>

#include "ui_stockprintwidget.h"

namespace Ui {
class StockAddWidget;
}

class StockAddWidget : public QWidget{
    Q_OBJECT

public:
    explicit StockAddWidget(QWidget *parent = 0);
    ~StockAddWidget();

    int AddStock();
    int ShowAllStock();
    QString catchCode();

private slots:
    void on_BackButton_clicked();

    void on_AddButton_clicked();

    void on_CheckButton_clicked();

private:
    Ui::StockAddWidget *ui;

    QSqlTableModel *model;
};


#endif // STOCKADDWIDGET_H
