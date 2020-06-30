#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>

namespace Ui {
class MenuWidget;
}

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = 0);
    ~MenuWidget();

private slots:

    void on_UserModifyButton_clicked();

    void on_UserDeleteButton_clicked();

    void on_StockModifyButton_clicked();

    void on_ManageButton_clicked();

    void on_LogoutButton_clicked();

    void on_ExitButton_clicked();

private:
    Ui::MenuWidget *ui;
};

#endif // MENUWIDGET_H
