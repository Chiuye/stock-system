#ifndef USERMODIFYCODEWIDGET_H
#define USERMODIFYCODEWIDGET_H

#include <QWidget>

namespace Ui {
class UserModifyCodeWidget;
}

class UserModifyCodeWidget : public QWidget{
    Q_OBJECT

public:
    explicit UserModifyCodeWidget(QWidget *parent = 0);
    ~UserModifyCodeWidget();

    int SaveCode( QString );
    int match( QString );

private slots:
    void on_BackButton_clicked();

    void on_SaveButton_clicked();

private:
    Ui::UserModifyCodeWidget *ui;
};

#endif // USERMODIFYCODEWIDGET_H
