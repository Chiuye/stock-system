#include "userinfodelete.h"
#include "ui_userinfodelete.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "global.h"
#include "menuwidget.h"
#include "demo0.h"

UserInfoDelete::UserInfoDelete(QWidget *parent) : QWidget(parent),
                        ui(new Ui::UserInfoDelete){
    ui->setupUi(this);
}

UserInfoDelete::~UserInfoDelete(){
    delete ui;
}

void UserInfoDelete::on_BackButton_clicked(){
    MenuWidget* m0 = new MenuWidget;
    m0->show();
    this->close();
}

void UserInfoDelete::on_EnsureButton_clicked(){
    //询问对话框
    int ret = QMessageBox::question( this, "呜呜>-<", "最后一次，您确定要注销账户吗0_0?", "确定", "取消" );
    if( ret == 0 ){
        int flag = 100;
        flag = DelUserInfo( g_account );

        switch( flag ){
        case 100:
           QMessageBox::warning( this, "错误＞﹏＜", "呜呜，出现了未知错误……", "好吧");
           break;
        case 1:
            QMessageBox::warning( this, "错误＞﹏＜", "呜呜，打不开数据库，恐怕暂时没法删除了……", "好吧");
            break;
        case 2:
            QMessageBox::warning( this, "错误＞﹏＜", "呜呜，不知道为什么，删除数据失败了……", "好吧");
            break;
        case 0://删除成功
            QMessageBox::information( this, "期待下次相见>_<", "成功删除账户，即将返回主界面……", "再见~" );
            //切换窗口
            demo0* m0 = new demo0;
            m0->show();
            this->close();
        }
    }
}

int UserInfoDelete::DelUserInfo( QString _account ){
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
        QString sql;    QSqlQuery query(db);
        sql = QString( "DELETE FROM users WHERE account = '%1' " ).arg( _account );
        bool ok = query.exec( sql );

        if( ok ){//成功删除
            //清空用户信息
            g_account = "";
            g_password = "";
            g_name = "";
            g_phone = "";
            g_isvip = false;

            return 0;
        }
        //删除失败
        else return 2;
    }
}
