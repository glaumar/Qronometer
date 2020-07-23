#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(&timer_, &QTimer::timeout, this, &MainWindow::setTimeLabel);
    timer_.setInterval(11);
    ui->timeNumber->display("00:00.000");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTimeLabel()
{
    int msecs = elapsed_timer_.elapsed() + ela_msecs_;
    QTime time = QTime::fromMSecsSinceStartOfDay(msecs);
    ui->timeNumber->display(time.toString("mm:ss.zzz"));
}

void MainWindow::start()
{
    timer_.start();
    elapsed_timer_.start();
    time_status_ = MainWindow::STARTED;
}

void MainWindow::pause()
{
    ela_msecs_ += elapsed_timer_.elapsed();
    timer_.stop();
    time_status_ = MainWindow::PAUSED;
}

void MainWindow::resume()
{
    timer_.start();
    elapsed_timer_.restart();
    time_status_ = MainWindow::STARTED;
}

void MainWindow::stop()
{
   ela_msecs_ = 0;
   timer_.stop();
   ui->startButton->setText("Start");
}


void MainWindow::on_startButton_clicked()
{
    switch (time_status_) {
    case MainWindow::PREPARED:
        start();
        ui->startButton->setText("Pause");
        break;
    case MainWindow::STARTED:
        pause();
        ui->startButton->setText("Resume");
        break;
    case MainWindow::PAUSED:
        resume();
        ui->startButton->setText("Pause");
        break;
    }
}

void MainWindow::on_stopButton_clicked()
{
    stop();
    time_status_ = MainWindow::PREPARED;
    ui->timeNumber->display("00:00.000");
}
