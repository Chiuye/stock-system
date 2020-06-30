#include "menuwidget.h"
#include "ui_menuwidget.h"
#include <QMessageBox>

#include "global.h"
#include "usermanagewidget.h"
#include "userinfomodify.h"
#include "userinfodelete.h"
#include "stockmodifywidget.h"
#include "demo0.h"

MenuWidget::MenuWidget(QWidget *parent) : QWidget(parent),
                    ui(new Ui::MenuWidget){
    ui->setupUi(this);


    if( g_isvip == true ){
        ui->WelLabel->setStyleSheet("color:#5b00e6;");
        ui->WelLabel->setText( "欢迎：【管理员】" + g_name );
    }
    else{
        ui->WelLabel->setStyleSheet("color:#0066cc;");
        ui->WelLabel->setText( "欢迎：【普通用户】" + g_name );
    }
}

MenuWidget::~MenuWidget(){
    delete ui;
}

void MenuWidget::on_UserModifyButton_clicked(){
    UserInfoModify* m0 = new UserInfoModify;
    m0->show();
    this->close();
}

void MenuWidget::on_UserDeleteButton_clicked(){
    UserInfoDelete* m0 = new UserInfoDelete;
    m0->show();
    this->close();
}

void MenuWidget::on_StockModifyButton_clicked(){
    StockModifyWidget* m0 = new StockModifyWidget;
    m0->show();
    this->close();
}

void MenuWidget::on_ManageButton_clicked(){
    if( g_isvip == false ){
        QMessageBox::information( this, "啊哦>-<", "抱歉，这是只有管理员才能使用的功能哟~", "了解~" );
    }
    else {
        UserManageWidget* m0 = new UserManageWidget;
        m0->show();
        this->close();
    }
}

void MenuWidget::on_LogoutButton_clicked(){
    //询问对话框
    int ret = QMessageBox::question( this, "0..0", "你确定要重新登录嘛?", "确定", "取消" );
    switch( ret ){
    default :
        break;
    case 0 :
        //清空用户信息
        g_account = "";
        g_password = "";
        g_name = "";
        g_phone = "";
        g_isvip = false;
        //跳转界面
        demo0* m0 = new demo0;
        m0->show();
        this->close();
    }
}

void MenuWidget::on_ExitButton_clicked(){
    //询问对话框
    int ret = QMessageBox::question( this, "0_0", "你确定要退出嘛?", "确定", "取消" );
    switch( ret ){
    default :
        break;
    case 0 :
        this->close();
    }
}
