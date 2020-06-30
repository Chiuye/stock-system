#ifndef STOCKFORECASTWIDGET_H
#define STOCKFORECASTWIDGET_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class StockForecastWidget;
}

class StockForecastWidget : public QWidget{
    Q_OBJECT

public:
    explicit StockForecastWidget(QWidget *parent = 0);
    ~StockForecastWidget();

    int getprices( QString );
    void calculate();
    void paint();

private slots:
    void on_BackButton_clicked();

private:
    Ui::StockForecastWidget *ui;

    QImage image;
    static const int n = 30;//数据个数
    float prices[n];
    int maxnum, minnum;
    float avr, result;
    double a, b;//线性回归方程参数


protected:
    //重载绘图事件（虚函数）
    //当窗口状态改变的时候，自动调用绘图事件，重新绘图
    void paintEvent( QPaintEvent * );

};

#endif // STOCKFORECASTWIDGET_H
