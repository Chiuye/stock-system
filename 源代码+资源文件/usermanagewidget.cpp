#include "usermanagewidget.h"
#include "ui_usermanagewidget.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "global.h"
#include "menuwidget.h"

UserManageWidget::UserManageWidget(QWidget *parent) : QWidget(parent),
                        ui(new Ui::UserManageWidget){
    ui->setupUi(this);

    int flag = 100;
    flag = linkDatabase();

    switch( flag ){
    case 100:
       QMessageBox::warning( this, "错误＞﹏＜", "呜呜，出现了未知错误……", "好吧");
       break;
    case 1:
        QMessageBox::warning( this, "错误＞﹏＜", "呜呜，打不开数据库，恐怕没法查看了……", "好吧");
        break;
    case 0:
        break;
    }
}

UserManageWidget::~UserManageWidget(){
    delete ui;
}

void UserManageWidget::on_BackButton_clicked(){
    MenuWidget* m0 = new MenuWidget;
    m0->show();
    this->close();
}

int UserManageWidget::linkDatabase(){
    //显示所有用户信息

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
        model->setTable( "users" );
        //把model放在view中
        ui->tableView->setModel( model );
        //显示model里的数据
        model->select();
        //修改表头
        model->setHeaderData( 0, Qt::Horizontal, "用户名" );
        model->setHeaderData( 1, Qt::Horizontal, "密码" );
        model->setHeaderData( 2, Qt::Horizontal, "姓名" );
        model->setHeaderData( 3, Qt::Horizontal, "手机号" );
        model->setHeaderData( 4, Qt::Horizontal, "管理员" );
        //设置编辑格式
        ui->tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
        ui->tableView->setSelectionMode( QAbstractItemView::SingleSelection );
        ui->tableView->setEditTriggers( QAbstractItemView::NoEditTriggers );
        ui->tableView->setColumnWidth( 3, 200 );
        ui->tableView->setSortingEnabled( true );

        return 0;
    }
}
