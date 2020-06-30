#include "demo0.h"
#include "ui_demo0.h"
#include "signupwidget.h"
#include "loginwidget.h"

demo0::demo0(QWidget *parent) : QWidget(parent),
                            ui(new Ui::demo0){
    ui->setupUi(this);
}

demo0::~demo0(){
    delete ui;
}

void demo0::on_ExitButton_clicked(){
    this->close();
}

void demo0::on_SignupButton_clicked(){
    SignupWidget* m0 = new SignupWidget;
    m0->show();
    this->close();
}

void demo0::on_LoginButton_clicked(){
    LoginWidget* m0 = new LoginWidget;
    m0->show();
    this->close();
}
