#include "mainwindow.h"

#include <QApplication>
#include <QTimer>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("QChronometer");

    w.show();
    return a.exec();
}
