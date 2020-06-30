#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();

    int userLogin(  QString, QString );

private slots:
    void on_LoginButton1_clicked();

    void on_BackButton1_clicked();

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
