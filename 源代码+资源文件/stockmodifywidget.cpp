#include "stockmodifywidget.h"
#include "ui_stockmodifywidget.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtDebug>

#include "global.h"
#include "menuwidget.h"
#include "stockaddwidget.h"
#include "stockprintwidget.h"
#include "stockforecastwidget.h"

StockModifyWidget::StockModifyWidget(QWidget *parent) : QWidget(parent),
                            ui(new Ui::StockModifyWidget){
    ui->setupUi(this);
    ui->radioButton->setChecked(true);
    ui->radioButton_2->setChecked(true);

    glink = 0;

    int flag = 100;
    flag = linkdb();

    switch( flag ){
    case 100:
       QMessageBox::warning( this, "错误＞﹏＜", "呜呜，出现了未知错误……", "好吧");
       break;
    case 1:
        QMessageBox::warning( this, "错误＞﹏＜", "呜呜，打不开数据库，恐怕没法使用了……", "好吧");
        break;
    case 0:
        ShowStocks();
        break;
    }

}

StockModifyWidget::~StockModifyWidget(){
    delete ui;
}

void StockModifyWidget::on_BackButton_clicked(){
    MenuWidget* m0 = new MenuWidget;
    m0->show();
    this->close();
}

void StockModifyWidget::on_DeleteButton_clicked(){
    //询问对话框
    int ret = QMessageBox::question( this, ">-<", "你确定要删除选中的股票吗?", "确定", "取消" );
    if( ret == 0 ){
        int flag = DelStocks();
        switch( flag ){
        case 1:
            QMessageBox::information( this, "错误＞﹏＜", "呜呜，不知道为什么，删除数据失败了……", "好吧" );
            break;
        case 2:
            QMessageBox::information( this, "啊咧咧……？", "你好像还没有选中任何股票喔？", "知道啦" );
            break;
        case 0:
            QMessageBox::information( this, "完成XD", "该股票已成功删除啦~", "好哒" );
        }
    }
}

void StockModifyWidget::on_ForecastButton_clicked(){

    QString StockCode = catchCode();

    if ( StockCode == "" )
        QMessageBox::information( this, "啊咧咧……？", "你好像还没有选中任何股票喔？", "知道啦" );
    else{
        g_stockcode = StockCode;
        //切换窗口
        StockForecastWidget* m0 = new StockForecastWidget;
        m0->show();
        this->close();
    }
}

void StockModifyWidget::on_AddButton_clicked(){
    //g_stockcode = "";   g_stockname = "";

    StockAddWidget* m0 = new StockAddWidget;
    m0->show();
    this->close();
}

void StockModifyWidget::on_CheckButton_clicked(){
    QString StockCode = catchCode();

    if ( StockCode == "" )
        QMessageBox::information( this, "啊咧咧……？", "你好像还没有选中任何股票喔？", "知道啦" );
    else{
        g_stockcode = StockCode;
        glink = 2;
        //切换窗口
        StockPrintWidget* m0 = new StockPrintWidget;
        m0->show();
        this->close();
    }
}

int StockModifyWidget::linkdb(){
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
        db.exec( "SET NAMES 'GBK'" );   //防止中文乱码
        return 0;
    }
}

int StockModifyWidget::ShowStocks(){
    //显示用户股票信息
        //设置模型
        model = new QSqlQueryModel( ui->tableView );
        QString str;
        if( ui->radioButton_2->isChecked() ){
            str = QString( "SELECT userstocks.stockcode, stocks.stockname \
                            FROM userstocks \
                            INNER JOIN stocks \
                            ON userstocks.stockcode = stocks.stockcode \
                            WHERE userstocks.account = '%1' \
                            ORDER BY stockname ASC;" ).arg( g_account );
        }
        else{
            str = QString( "SELECT userstocks.stockcode, stocks.stockname \
                            FROM userstocks \
                            INNER JOIN stocks \
                            ON userstocks.stockcode = stocks.stockcode \
                            WHERE userstocks.account = '%1' \
                            ORDER BY stockcode ASC;" ).arg( g_account );
        }
        model->setQuery( str );
        //修改表头
        model->setHeaderData( 0, Qt::Horizontal, "股票代码" );
        model->setHeaderData( 1, Qt::Horizontal, "股票名称" );
        //设置编辑格式：选中单行、不可编辑
        //ui->tableView->verticalHeader()->hide();//不显示序号
        ui->tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
        ui->tableView->setSelectionMode( QAbstractItemView::SingleSelection );
        ui->tableView->setEditTriggers( QAbstractItemView::NoEditTriggers );

        //把model放在view中
        ui->tableView->setModel( model );

        return 0;
}

int StockModifyWidget::DelStocks(){
    QString StockCode = catchCode();

    if ( StockCode == "" ) return 2;
    else{
        qDebug() << "要删除的股票代码为：" << StockCode;
        QString str = QString( "DELETE FROM userstocks \
                                WHERE stockcode = '%1' \
                                AND account = '%2'" ).arg(StockCode).arg(g_account);
        QSqlQuery query;
        bool ok = query.exec( str );
        //删除成功
        if( ok ) {
            //重新显示
            ShowStocks();

            return 0;
        }
        //删除失败
        else return 1;
    }

}

QString StockModifyWidget::catchCode(){
    //获取选中行第一列的内容（即股票代码）
    int row = ui->tableView->currentIndex().row();
    QModelIndex index = model->index( row, 0 );
    QString StockCode = model->data( index ).toString();
    qDebug() << "选中的股票代码是：" << StockCode;
    //获取选中行第二列的内容（即股票名称）
    g_stockname = model->data( model->index( row, 1 ) ).toString();
    qDebug() << "选中的股票名称是：" << g_stockname;
    return StockCode;
}

void StockModifyWidget::on_reShowButton_clicked(){
    ShowStocks();
}
