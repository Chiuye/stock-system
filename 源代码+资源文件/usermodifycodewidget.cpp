#include "usermodifycodewidget.h"
#include "ui_usermodifycodewidget.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "global.h"
#include "userinfomodify.h"

#include <regex>//正则表达式
using namespace std;

UserModifyCodeWidget::UserModifyCodeWidget(QWidget *parent) : QWidget(parent),
                        ui(new Ui::UserModifyCodeWidget){
    ui->setupUi(this);
    //不显示明文密码
    ui->Password_old->setEchoMode( QLineEdit::Password );
    ui->Password_new1->setEchoMode( QLineEdit::Password );
    ui->Password_new2->setEchoMode( QLineEdit::Password );
}

UserModifyCodeWidget::~UserModifyCodeWidget(){
    delete ui;
}

void UserModifyCodeWidget::on_BackButton_clicked(){
    UserInfoModify* m0 = new UserInfoModify;
    m0->show();
    this->close();
}

void UserModifyCodeWidget::on_SaveButton_clicked(){
    //把用户输入的的信息取出
    QString password0 = ui->Password_old->text();
    QString password1 = ui->Password_new1->text();
    QString password2 = ui->Password_new2->text();
    //旧密码必须正确
    if( password0 != g_password ){
        QMessageBox::warning( this, "糟糕X﹏X", "旧密码不对喔……再仔细想想呀", "好吧" );
    }
    //新密码不能为空
    else if( password1 == "" ){
        QMessageBox::warning( this, "不可以>x<", "新密码不能为空哟~", "知道啦" );
    }
    //密码与确认密码相同
    else if( password1 != password2 ){
        QMessageBox::warning( this, "不对劲qaq", "输入的密码与确认密码不同，再检查一下吧w", "好的" );
    }
    //新密码符合格式后插入数据库
    else if( match( password2 ) == 0 ){
        int flag = 100;
        flag = SaveCode( password1 );
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
        case 0://插入成功
            //清空窗口的输入
            ui->Password_old->clear();
            ui->Password_new1->clear();
            ui->Password_new2->clear();
            QMessageBox::information( this, "XD", "密码更新成功w~要记牢哟", "好哒" );
        }
    }
}

int UserModifyCodeWidget::SaveCode( QString _password ){
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
        QSqlQuery query(db);    QString sql;
        sql = QString( "UPDATE users SET password = '%1' WHERE account = '%2' ")
           .arg(_password).arg(g_account);
        bool ok = query.exec( sql );
        //更新成功
        if( ok ){
            g_password = _password;
            return 0;
        }
        //更新失败
        else return 2;
    }

}

int UserModifyCodeWidget::match( QString _password ){
    //检查格式
    bool flag2;
    //把QString转换成String
    string _password0 = _password.toStdString();

    //密码，6-18位，必须同时含有字母和数字，支持下划线
    flag2 = regex_match( _password0, regex( "^(?![0-9]+$)(?![a-zA-Z]+$)[0-9A-Za-z_]{6,18}$" ) );

    if( flag2 == false ){
        QMessageBox::warning( this, "不对劲qaq", "新密码不符合格式喔，再检查一下吧w", "好的" );
        return 2;
    }
    else return 0;
}
