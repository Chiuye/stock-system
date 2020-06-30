#ifndef STOCKMODIFYWIDGET_H
#define STOCKMODIFYWIDGET_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class StockModifyWidget;
}

class StockModifyWidget : public QWidget{
    Q_OBJECT

public:
    explicit StockModifyWidget(QWidget *parent = 0);
    ~StockModifyWidget();

    int linkdb();
    int ShowStocks();
    int DelStocks();
    QString catchCode();

private slots:
    void on_BackButton_clicked();

    void on_DeleteButton_clicked();

    void on_ForecastButton_clicked();

    void on_AddButton_clicked();

    void on_CheckButton_clicked();

    void on_reShowButton_clicked();

private:
    Ui::StockModifyWidget *ui;

    QSqlQueryModel *model;
};

#endif // STOCKMODIFYWIDGET_H
