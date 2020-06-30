#ifndef USERINFODELETE_H
#define USERINFODELETE_H

#include <QWidget>

namespace Ui {
class UserInfoDelete;
}

class UserInfoDelete : public QWidget{
    Q_OBJECT

public:
    explicit UserInfoDelete(QWidget *parent = 0);
    ~UserInfoDelete();

    int DelUserInfo( QString );

private slots:
    void on_BackButton_clicked();

    void on_EnsureButton_clicked();

private:
    Ui::UserInfoDelete *ui;
};

#endif // USERINFODELETE_H
