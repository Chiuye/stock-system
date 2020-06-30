#ifndef STOCKPRINTWIDGET_H
#define STOCKPRINTWIDGET_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class StockPrintWidget;
}

class StockPrintWidget : public QWidget{
    Q_OBJECT

public:
    explicit StockPrintWidget(QWidget *parent = 0);
    ~StockPrintWidget();

    int showPrices();

private slots:
    void on_BackButton_clicked();

private:
    Ui::StockPrintWidget *ui;

    QSqlQueryModel *model;
};

#endif // STOCKPRINTWIDGET_H
