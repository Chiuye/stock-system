#include "userinfomodify.h"
#include "ui_userinfomodify.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "global.h"
#include "menuwidget.h"
#include "usermodifycodewidget.h"

#include <regex>//正则表达式
using namespace std;

UserInfoModify::UserInfoModify(QWidget *parent) : QWidget(parent),
                            ui(new Ui::UserInfoModify){
    ui->setupUi(this);
    //将用户信息显示在界面上
    ui->Account->setText( g_account );
    ui->Name->setText( g_name );
    ui->Phone->setText( g_phone );
}

UserInfoModify::~UserInfoModify(){
    delete ui;
}

void UserInfoModify::on_BackButton_clicked(){
    MenuWidget* m0 = new MenuWidget;
    m0->show();
    this->close();
}

void UserInfoModify::on_PasswordButton_clicked(){
    UserModifyCodeWidget* m0 = new UserModifyCodeWidget;
    m0->show();
    this->close();
}

void UserInfoModify::on_SaveButton_clicked(){
    //把用户输入的的信息取出
    QString account = ui->Account->text();
    QString name = ui->Name->text();
    QString phone = ui->Phone->text();
    //输入的数据都不能为空
    if( account == "" || name == "" || phone == "" ){
        QMessageBox::warning( this, "不可以>x<", "存在空项，必须把所有信息填上哟~", "知道啦" );
    }
    else if( match( account, phone ) == 0 ){
         //把用户信息输入数据库
         int flag = 100;
         flag = SaveUserInfo( account, name, phone );

         switch( flag ){
         case 100:
            QMessageBox::warning( this, "错误＞﹏＜", "呜呜，出现了未知错误……", "好吧");
            break;
         case 1:
             QMessageBox::warning( this, "错误＞﹏＜", "呜呜，打不开数据库，恐怕没法更改了……", "好吧");
             break;
         case 2:
             QMessageBox::warning( this, "错误＞﹏＜", "呜呜，不知道为什么，更新失败了……", "好吧");
             break;
         case 3:
             QMessageBox::information( this, "小提示(*^-^*)", "这个用户名已经被抢先注册了，换一个吧~", "好哒" );
             break;
         case 0://插入成功
             QMessageBox::information( this, ">w<", "保存用户信息成功啦w", "耶" );
         }
     }
}

int UserInfoModify::SaveUserInfo( QString _account, QString _name, QString _phone ){
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
        QSqlQuery query(db);
        db.exec( "SET NAMES 'GBK'" );   //防止中文乱码
        //如果修改了用户名
        if( g_account != _account ){
            //查重
            query.exec( "SELECT account FROM users" );
            while( query.next() ){
                QString account= query.value(0).toString();
                if( _account.compare( account )==0 ){
                    return 3;//出现重复
                }
            }
        }
        //没有重复，

        //可以更新数据库啦XD
        QString sql;
        sql = QString( "UPDATE users SET account = '%0', name = '%1', phone = '%2'"
                       "WHERE account = '%3' ")
           .arg(_account).arg(_name).arg(_phone).arg(g_account);
        bool ok = query.exec( sql );
        //更新成功
        if( ok ){
            g_account = _account;
            g_name = _name;
            g_phone = _phone;
            return 0;
        }
        //更新失败
        else return 2;
    }
}

int UserInfoModify::match( QString _account, QString _phone ){
    //检查格式
    bool flag1, flag3;
    //把QString转换成String
    string _account0 = _account.toStdString();
    string _phone0 = _phone.toStdString();

    //用户名，必须字母开头，支持字母、数字、下划线
    flag1 = regex_match( _account0, regex( "^[a-zA-Z][A-Za-z0-9_]{0,}" ) );
    //手机号码，必须11位数字
    flag3 = regex_match( _phone0, regex( "^[0-9]{11}$" ) );

    if( flag1 == false ){
        QMessageBox::warning( this, "不对劲qaq", "用户名不符合格式喔，再检查一下吧w", "好的" );
        return 1;
    }
    else if( flag3 == false ){
        QMessageBox::warning( this, "不对劲qaq", "手机号不符合格式喔，再检查一下吧w", "好的" );
        return 3;
    }
    else return 0;
}
