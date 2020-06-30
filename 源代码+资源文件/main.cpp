#include "demo0.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    demo0 w;
    w.show();

    return a.exec();
}
