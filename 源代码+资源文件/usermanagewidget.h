#ifndef USERMANAGEWIDGET_H
#define USERMANAGEWIDGET_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class UserManageWidget;
}

class UserManageWidget : public QWidget{
    Q_OBJECT

public:
    explicit UserManageWidget(QWidget *parent = 0);
    ~UserManageWidget();

    int linkDatabase();

private slots:
    void on_BackButton_clicked();

private:
    Ui::UserManageWidget *ui;

    QSqlTableModel *model;
};

#endif // USERMANAGEWIDGET_H
