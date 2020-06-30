#include "stockforecastwidget.h"
#include "ui_stockforecastwidget.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

#include "global.h"
#include "stockmodifywidget.h"

StockForecastWidget::StockForecastWidget(QWidget *parent) : QWidget(parent),
                        ui(new Ui::StockForecastWidget){
    ui->setupUi(this);

    int flag = 100;
    flag = getprices( g_stockcode );
    calculate();

    ui->label->setText( g_stockname + " （" + g_stockcode + "） 的收盘价走势如下哟："  );
    ui->label2->setStyleSheet("color:#ff4004;");
    ui->label2->setText( QString::number(result) );

    switch( flag ){
    case 100:
       QMessageBox::warning( this, "错误＞﹏＜", "呜呜，出现了未知错误……", "好吧");
       break;
    case 1:
        QMessageBox::warning( this, "错误＞﹏＜", "呜呜，打不开数据库，恐怕没法使用了……", "好吧");
        break;
    case 2:
        QMessageBox::warning( this, "错误＞﹏＜", "呜呜，读取数据失败了……", "好吧");
        break;
    case 0:
        break;
    }
    //设置画布的初始化大小，使用32位颜色
    image = QImage( 1000, 600, QImage::Format_RGB32);
    //画布初始化背景色使用白色
    QColor backColor = qRgb(255,255,255);
    //对画布进行填充
    image.fill( backColor );

    if( flag == 0 ) paint();
}

StockForecastWidget::~StockForecastWidget(){
    delete ui;
}

void StockForecastWidget::on_BackButton_clicked(){
    StockModifyWidget* m0 = new StockModifyWidget;
    m0->show();
    this->close();
}

int StockForecastWidget::getprices( QString _stockcode ){
    //连接数据库
    QSqlDatabase db;
    if( QSqlDatabase::contains( "qt_sql_default_connection" ) )
        db = QSqlDatabase::database( "qt_sql_default_connection" );
    else
        db = QSqlDatabase::addDatabase("QMYSQL");
    //打开数据库
    db.setHostName("localhost");
    db.setUserName( sqluser );
    db.setPassword( sqlpass );
    db.setDatabaseName("stock-system");
    //打开失败
    if( !db.open() )    return 1;
    //打开成功
    else {
        QString str = QString( "SELECT Close FROM stocksinfo \
                                WHERE stockcode = '%1'" ).arg(_stockcode);
        QSqlQuery query;
        bool ok = query.exec( str );
        //读取成功
        if( ok ) {
            float temp;
            //创建存储结果的列表
            QVector <float> array;
            //如果有数据，取第一列添加到array
            while( query.next() ){
               temp = query.value(0).toFloat();
               array.append( temp );
            }
            //如果数组非空
            if( !array.empty() ){
                for( int i = 0; i < n; i++ ){
                    prices[i] = array.at(i);//给成员prices赋值
                    qDebug() << prices[i];
                }
            }
            return 0;
        }
        //读取失败
        else return 2;
    }
}

void StockForecastWidget::paintEvent( QPaintEvent * ){
    QPainter painter0( this );
    painter0.drawImage( 5, 80, image );
}

void StockForecastWidget::calculate(){
    //计算最大值、最小值、平均值
    maxnum = 0; minnum = 0; avr = 0;
    float sum = 0;
    for( int i = 0; i < n; i++ ){
        sum += prices[i];
        if( prices[i] > prices[maxnum] )   maxnum = i;
        if( prices[i] < prices[minnum] )   minnum = i;
    }
    avr = sum/n;
    qDebug() << maxnum << minnum << avr;

    //计算线性回归方程，y=bx+a
    int n0 = 12;//取前n0天的数据用于预测
    float avr1, sum1 = 0, avr2, sum2 = 0;
    for( int i = n-n0; i < n; i++ ){
        sum1 += i;
        sum2 += prices[i];
    }
    avr1 = sum1/n0;//x平均值
    avr2 = sum2/n0;//y平均值

    double b1 = 0, b2 = 0;//b的分子、分母
    for( int i = n-n0; i < n; i++ ){
        b1 += ( i-avr1 )*( prices[i]-avr2 );
        b2 += ( i-avr1 )*( i-avr1 );
    }
    b = b1/b2;  a = avr2 - b*avr1;
    result = b*n + a;
}

void StockForecastWidget::paint(){
    QPainter painter( &image );
    //设置反锯齿模式
    painter.setRenderHint( QPainter::Antialiasing, true );
    //原点坐标
    int x0 = 50, y0 = 570;
    //x轴宽度、y轴高度
    int width = 850, height = 550;
    //x轴起点、终点坐标
    painter.drawLine( x0, y0, x0+width, y0 );
    //y轴起点、终点坐标
    painter.drawLine( x0, y0, x0, y0-height );

    float c = 120;//用于调整位置的常数
    double kx = ( double ) width /( n-1 );//x轴的系数
    double ky = ( double ) height /( prices[maxnum] );//y方向的比例系数

    QPen pen, penPoint;
    pen.setColor( Qt::black );      pen.setWidth(2);
    penPoint.setColor( Qt::blue );    penPoint.setWidth(5);

    for( int i = 0; i < n-1; i++ ){
        painter.setPen( penPoint );//蓝色的笔，用于标记各个点
        painter.drawPoint( x0+kx*i, y0-(prices[i]*ky-c) );
        painter.setPen( pen );//黑色笔用于连线
        painter.drawLine( x0+kx*i, y0-(prices[i]*ky-c), x0+kx*(i+1), y0-(prices[i+1]*ky-c) );
    }
    painter.setPen( penPoint );
    painter.drawPoint( x0 + kx*(n-1), y0-(prices[n-1]*ky-c) );//绘制最后一个点

    //绘制平均线
    QPen penAvr;
    penAvr.setColor( Qt::darkGreen );
    penAvr.setWidth(1);
    penAvr.setStyle( Qt::DotLine );//线条类型为虚线
    painter.setPen( penAvr );
    painter.drawLine( x0, y0-(avr*ky-c), x0+width, y0-(avr*ky-c) );

    //绘制最大值和最小值
    QPen penMaxMin;
    penMaxMin.setColor(Qt::darkGreen);//暗绿色
    painter.setPen( penMaxMin );
    painter.drawText( x0 + kx*maxnum-kx, y0 -( prices[maxnum]*ky-c+15 ),
                        "最大值" + QString::number( prices[maxnum] ) );
    painter.drawText( x0 + kx*minnum-kx, y0 -( prices[minnum]*ky-c-25 ),
                        "最小值"+QString::number( prices[minnum] ));
    penMaxMin.setColor(Qt::red);
    penMaxMin.setWidth(7);
    painter.setPen(penMaxMin);
    //标记最大、小值点
    painter.drawPoint( x0+kx*maxnum, y0-(prices[maxnum]*ky-c) );
    painter.drawPoint( x0+kx*minnum, y0-(prices[minnum]*ky-c) );

    //绘制刻度线
    QPen penDegree;
    penDegree.setColor( Qt::black );
    penDegree.setWidth(2);
    painter.setPen( penDegree );

    //x轴刻度线
    int n1 = 10;//分成n1份
    for(int i = 0; i <= n1; i++ ){
        //绘制一段长度为10的直线，表示刻度
        painter.drawLine( x0+i*width/n1, y0, x0+i*width/n1, y0-10);
        painter.drawText( x0+i*width/n1-6, y0+15, QString::number(i*n/n1) );
    }

    //y轴刻度线
    int n2 = 15;//把y轴分成n2份
    for( int i = 0; i <= n2; i++ ){
        painter.drawLine( x0, y0-i*height/n2, x0+8, y0-i*height/n2 );
    }

}
