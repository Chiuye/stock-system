#include "signupwidget.h"
#include "ui_signupwidget.h"

#include <QString>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "global.h"
#include "demo0.h"

#include <regex>//正则表达式
using namespace std;

SignupWidget::SignupWidget(QWidget *parent) : QWidget(parent),
                                    ui(new Ui::SignupWidget){
    ui->setupUi(this);
    //不显示明文密码
    ui->Password1->setEchoMode( QLineEdit::Password );
    ui->Password2->setEchoMode( QLineEdit::Password );
}

SignupWidget::~SignupWidget(){
    delete ui;
}

void SignupWidget::on_BackButton1_clicked(){
    demo0* m0 = new demo0;
    m0->show();
    this->close();
}

void SignupWidget::on_SignupButton_clicked(){
    //把用户输入的的信息取出
    QString account = ui->Account->text();
    QString name = ui->Name->text();
    QString phone = ui->Phone->text();
    QString password1 = ui->Password1->text();
    QString password2 = ui->Password2->text();
    //输入的数据都不能为空
    if( account == "" || name == "" || phone == "" || password1 == "" ){
        QMessageBox::warning( this, "不可以>x<", "存在空项，必须把所有信息填上哟~", "知道啦" );
    }
    //密码与确认密码相同
    else if( password1 != password2 ){
        QMessageBox::warning( this, "不对劲qaq", "输入的密码与确认密码不同，再检查一下吧w", "好的" );
    }
    else { 
        //检查格式
        int flag0 = match( account, password1, phone);
        //可以注册
        if ( flag0 == 0 ){
            //把用户信息输入数据库
            int flag = 100;
            flag = AddUserInfo( account, password1, name, phone );

            switch( flag ){
            case 100:
               QMessageBox::warning( this, "错误＞﹏＜", "呜呜，出现了未知错误……", "好吧");
               break;
            case 1:
                QMessageBox::warning( this, "错误＞﹏＜", "呜呜，打不开数据库，恐怕没法使用了……", "好吧");
                break;
            case 2:
                QMessageBox::warning( this, "错误＞﹏＜", "呜呜，不知道为什么，插入数据库失败了……", "好吧");
                break;
            case 3:
                QMessageBox::information( this, "小提示(*^-^*)", "这个用户名已经被抢先注册了，换一个吧~", "好哒" );
                break;
            case 0://插入成功
                //清空窗口的输入
                ui->Account->clear();
                ui->Name->clear();
                ui->Phone->clear();
                ui->Password1->clear();
                ui->Password2->clear();
                //消息弹窗
                QMessageBox::information( this, "欢迎>w<", "注册成功啦wwwww!", "耶" );
                //切换窗口
                demo0* m0 = new demo0;
                m0->show();
                this->close();
            }
        }
    }
}

int SignupWidget::AddUserInfo( QString _account, QString _password, QString _name, QString _phone  ){
    //连接数据库
    QSqlDatabase db;
    if( QSqlDatabase::contains( "qt_sql_default_connection" ) )
        db = QSqlDatabase::database( "qt_sql_default_connection" );
    else
        db = QSqlDatabase::addDatabase("QMYSQL");
    //打开数据库
    db.setHostName( "localhost" );
    db.setUserName( sqluser );
    db.setPassword( sqlpass );
    db.setDatabaseName( "stock-system" );
    //打开失败
    if( !db.open() )    return 1;
    //打开成功
    else {
        //查重
        QSqlQuery query(db);
        db.exec( "SET NAMES 'GBK'" );   //防止中文乱码
        query.exec( "SELECT account FROM users" );
        while( query.next() ){
            QString account= query.value(0).toString();
            if( _account.compare( account )==0 ){
                return 3;//出现重复
            }
        }
        //没有重复，可以把数据插入数据库啦XD
        QString sql;
        sql = QString( "INSERT INTO users VALUES ('%1', '%2' , '%3', '%4', '%5') " )
           .arg(_account).arg(_password).arg(_name).arg(_phone).arg("否");
        bool ok = query.exec( sql );
        //插入成功
        if( ok ) return 0;
        //插入失败
        else return 2;
    }
}

int SignupWidget::match( QString _account, QString _password, QString _phone){
    //检查格式
    bool flag1, flag2, flag3;
    //把QString转换成String
    string _account0 = _account.toStdString();
    string _password0 = _password.toStdString();
    string _phone0 = _phone.toStdString();

    //用户名，必须字母开头，支持字母、数字、下划线
    flag1 = regex_match( _account0, regex( "^[a-zA-Z][A-Za-z0-9_]{0,}" ) );
    //密码，6-18位，必须同时含有字母和数字，支持下划线
    flag2 = regex_match( _password0, regex( "^(?![0-9]+$)(?![a-zA-Z]+$)[0-9A-Za-z_]{6,18}$" ) );
    //手机号码，必须11位数字
    flag3 = regex_match( _phone0, regex( "^[0-9]{11}$" ) );

    if( flag1 == false ){
        QMessageBox::warning( this, "不对劲qaq", "用户名不符合格式喔，再检查一下吧w", "好的" );
        return 1;
    }
    else if( flag2 == false ){
        QMessageBox::warning( this, "不对劲qaq", "密码不符合格式喔，再检查一下吧w", "好的" );
        return 2;
    }
    else if( flag3 == false ){
        QMessageBox::warning( this, "不对劲qaq", "手机号不符合格式喔，再检查一下吧w", "好的" );
        return 3;
    }
    else return 0;
}
