#include "stockaddwidget.h"
#include "ui_stockaddwidget.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

#include "global.h"
#include "stockmodifywidget.h"
#include "stockprintwidget.h"

StockAddWidget::StockAddWidget(QWidget *parent) : QWidget(parent),
                            ui(new Ui::StockAddWidget){
    ui->setupUi(this);

    glink = 0;

    int flag = 100;
    flag = ShowAllStock();

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

StockAddWidget::~StockAddWidget(){
    delete ui;
}

void StockAddWidget::on_BackButton_clicked(){
    StockModifyWidget* m0 = new StockModifyWidget;
    m0->show();
    this->close();
}

void StockAddWidget::on_AddButton_clicked(){

        int flag = 100;
        flag = AddStock();

        switch( flag ){
        case 1:
            QMessageBox::information( this, "啊咧咧……？", "你好像还没有选中任何股票喔？", "知道啦");
            break;
        case 2:
            QMessageBox::information( this, "错误＞﹏＜", "呜呜，不知道为什么，插入数据库失败了……", "好吧");
            break;
        case 3:
            QMessageBox::information( this, "哎呀哎呀……", "你已经添加过这支股票了哟~", "知道啦");
            break;
        case 0://添加成功
            QMessageBox::information( this, "XD", "新股票添加成功w~", "好哒" );
        }
}

void StockAddWidget::on_CheckButton_clicked(){
    QString StockCode = catchCode();
    if ( StockCode == "" )
        QMessageBox::information( this, "啊咧咧……？", "你好像还没有选中任何股票喔？", "知道啦" );
    else{
        g_stockcode = StockCode;
        glink = 1;
        //切换窗口
        StockPrintWidget* m0 = new StockPrintWidget;
        m0->show();
        this->close();
    }
}

int StockAddWidget::ShowAllStock(){
    //显示所有股票信息

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
        //设置模型
        model = new QSqlTableModel( this );
        model->setTable( "stocks" );
        //把model放在view中
        ui->tableView->setModel( model );
        //修改表头
        model->setHeaderData( 0, Qt::Horizontal, "股票代码" );
        model->setHeaderData( 1, Qt::Horizontal, "股票名称" );
        //设置编辑格式：可排序、选中单行、不可编辑
        ui->tableView->setSortingEnabled( true );
        ui->tableView->setSelectionMode( QAbstractItemView::SingleSelection );
        ui->tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
        ui->tableView->setEditTriggers( QAbstractItemView::NoEditTriggers );

        //显示model里的数据
        model->select();

        return 0;
    }
}

int StockAddWidget::AddStock(){
    QString StockCode = catchCode();
    //如果没有选中任何行
    if ( StockCode == "" ) return 1;
    else{
        //查重
        QSqlQuery query;
        QString sql = QString( "SELECT stockcode FROM userstocks \
                              WHERE account = '%1';" ).arg(g_account);
        query.exec( sql );
        while( query.next() ){
            QString stockcode0 = query.value(0).toString();
            if( StockCode.compare( stockcode0 ) == 0 ){
                return 3;//出现重复
            }
        }
        //没有重复，可以添加
        QString sql2;
        sql2 = QString( "INSERT INTO userstocks VALUES ('%1', '%2') " )
           .arg(g_account).arg(StockCode);
        bool ok = query.exec( sql2 );
        //插入成功
        if( ok ) return 0;
        //插入失败
        else return 2;
    }
}

QString StockAddWidget::catchCode(){
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
