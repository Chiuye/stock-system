#include "loginwidget.h"
#include "ui_loginwidget.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>

#include "global.h"
#include "demo0.h"
#include "menuwidget.h"

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent),
                        ui(new Ui::LoginWidget){
    ui->setupUi(this);
    //不显示明文密码
    ui->Password1->setEchoMode( QLineEdit::Password );
}

LoginWidget::~LoginWidget(){
    delete ui;
}

void LoginWidget::on_LoginButton1_clicked(){
    //把用户输入的的信息取出
    QString account = ui->Account->text();
    QString password1 = ui->Password1->text();
    //输入的数据都不能为空
    if( account == "" || password1 == "" ){
        QMessageBox::warning( this, "不可以>x<", "存在空项，必须把所有信息填上哟~", "知道啦" );
    }
    else{//可以开始连接数据库
        int flag = userLogin( account, password1 );
        switch( flag ){
        case 1:
            QMessageBox::warning( this, "错误＞﹏＜", "呜呜，打不开数据库，恐怕没法使用了……", "好吧");
            break;
        case 2:
            QMessageBox::warning( this, "错误＞﹏＜", "用户名或密码错啦，再想想吧w", "好哒");
            break;
        case 0:
            QMessageBox::information( this, "欢迎>w<", "登录成功啦wwwww!", "耶" );
            MenuWidget* m0 = new MenuWidget;
            m0->show();
            this->close();
        }
    }
}

void LoginWidget::on_BackButton1_clicked(){
    demo0* m0 = new demo0;
    m0->show();
    this->close();
}

int LoginWidget::userLogin( QString _account, QString _password ){
    qDebug() << "用户名：" << _account <<endl
             << "密码：" << _password;
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
    else{
        //调取数据库中用户的数据
        QSqlQuery query(db);
        db.exec("SET NAMES 'GBK'");
        query.exec("SELECT account, password, name, phone, isvip FROM users");
        //校检是否存在该账户
        while( query.next() ){
            //循环取出一条数据库账户信息
            QString user = query.value(0).toString();
            QString pass = query.value(1).toString();
            //qDebug() << user << pass;
            //与用户输入数据比对
            if( _account.compare(user) == 0 && _password.compare(pass) == 0 ){
                //比对成功，取出用户信息

                g_account = _account;
                g_password = _password;
                g_name = query.value(2).toString();
                g_phone = query.value(3).toString();
                QString vip = query.value(4).toString();
                if( vip.compare( "是" ) == 0 )
                    g_isvip = true;
                qDebug() << "姓名：" << g_name <<endl
                         << "手机号：" << g_phone <<endl
                         << "是否为管理员：" << g_isvip;
                return 0;
            }
        }
        //比对失败
        return 2;
    }
}
