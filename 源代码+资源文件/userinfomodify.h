#ifndef USERINFOMODIFY_H
#define USERINFOMODIFY_H

#include <QWidget>

namespace Ui {
class UserInfoModify;
}

class UserInfoModify : public QWidget{
    Q_OBJECT

public:
    explicit UserInfoModify(QWidget *parent = 0);
    ~UserInfoModify();

    int SaveUserInfo( QString, QString, QString );
    int match( QString, QString );

private slots:
    void on_BackButton_clicked();

    void on_PasswordButton_clicked();

    void on_SaveButton_clicked();

private:
    Ui::UserInfoModify *ui;
};

#endif // USERINFOMODIFY_H
