#include "stockprintwidget.h"
#include "ui_stockprintwidget.h"

#include <QMessageBox>

#include "global.h"
#include "stockmodifywidget.h"
#include "stockaddwidget.h"

StockPrintWidget::StockPrintWidget(QWidget *parent) : QWidget(parent),
                            ui(new Ui::StockPrintWidget){
    ui->setupUi(this);

    ui->label->setText( g_stockname + " （" + g_stockcode + "） 的历史价格如下哟："  );

    if( glink == 0 )
        QMessageBox::warning( this, "错误＞﹏＜", "呜呜，窗口连接发生了问题……", "好吧" );

    int flag = 100;
    flag = showPrices();

    switch( flag ){
    case 100:
       QMessageBox::warning( this, "错误＞﹏＜", "呜呜，出现了未知错误……", "好吧");
       break;
    case 1:
        QMessageBox::warning( this, "错误＞﹏＜", "呜呜，打不开数据库，恐怕没法使用了……", "好吧");
        break;
    case 0:
        break;
    }
}

StockPrintWidget::~StockPrintWidget(){
    delete ui;
}

void StockPrintWidget::on_BackButton_clicked(){
    g_stockcode = "";   g_stockname = "";

    if( glink == 1 ) {
        StockAddWidget* m0 = new StockAddWidget;
        m0->show();     this->close();
    }
    else if( glink == 2 ){
        StockModifyWidget* m0 = new StockModifyWidget;
        m0->show();     this->close();
    }
}

int StockPrintWidget::showPrices(){
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
        //设置模型
        model = new QSqlQueryModel( ui->tableView );
        QString str = QString( "SELECT stocksinfo.Date, stocksinfo.Open, stocksinfo.High, \
                                                      stocksinfo.Low, stocksinfo.Close \
                               FROM stocksinfo \
                               WHERE stocksinfo.stockcode = '%1' \
                               ORDER BY Date ASC;" ).arg( g_stockcode );
        model->setQuery( str );
        //修改表头
        model->setHeaderData( 0, Qt::Horizontal, "日期" );
        model->setHeaderData( 1, Qt::Horizontal, "开盘价" );
        model->setHeaderData( 2, Qt::Horizontal, "最高价" );
        model->setHeaderData( 3, Qt::Horizontal, "最低价" );
        model->setHeaderData( 4, Qt::Horizontal, "收盘价" );
        //设置编辑格式：选中单行、不可编辑
        //ui->tableView->verticalHeader()->hide();//不显示序号
        ui->tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
        ui->tableView->setSelectionMode( QAbstractItemView::SingleSelection );
        ui->tableView->setEditTriggers( QAbstractItemView::NoEditTriggers );

        //把model放在view中
        ui->tableView->setModel( model );

        return 0;
    }
}
