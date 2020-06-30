#ifndef SIGNUPWIDGET_H
#define SIGNUPWIDGET_H

#include <QWidget>

namespace Ui {
class SignupWidget;
}

class SignupWidget : public QWidget{
    Q_OBJECT

public:
    explicit SignupWidget(QWidget *parent = 0);
    ~SignupWidget();
    int AddUserInfo( QString, QString, QString, QString );
    int match( QString, QString, QString );

private slots:
    void on_BackButton1_clicked();
    void on_SignupButton_clicked();

private:
    Ui::SignupWidget *ui;
};

#endif // SIGNUPWIDGET_H
