#ifndef DEMO0_H
#define DEMO0_H

#include <QWidget>

namespace Ui {
class demo0;
}

class demo0 : public QWidget{
    Q_OBJECT

public:
    explicit demo0(QWidget *parent = 0);
    ~demo0();

private slots:
    void on_ExitButton_clicked();

    void on_SignupButton_clicked();

    void on_LoginButton_clicked();

private:
    Ui::demo0 *ui;
};

#endif // DEMO0_H
