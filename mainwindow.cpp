#include "mainwindow.h"

#include <QDebug>
#include <QTime>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QObject::connect(&timer_, &QTimer::timeout, this,
                     &MainWindow::setTimeLabel);
    timer_.setInterval(11);
    ui->timeNumber->display("00:00.000");

    model_.setHorizontalHeaderItem(0, new QStandardItem("#"));
    model_.setHorizontalHeaderItem(1, new QStandardItem("Total"));
    model_.setHorizontalHeaderItem(2, new QStandardItem("Lap"));
    model_.setHorizontalHeaderItem(3, new QStandardItem("Note"));
    ui->tableView->setModel(&model_);
    ui->tableView->setColumnWidth(0, 50);
    ui->tableView->setSortingEnabled(true);

    ui->lapButton->setEnabled(false);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setTimeLabel() {
    int msecs = elapsed_timer_.elapsed() + ela_msecs_;
    QTime time = QTime::fromMSecsSinceStartOfDay(msecs);
    ui->timeNumber->display(time.toString("mm:ss.zzz"));
}

void MainWindow::start() {
    timer_.start();
    elapsed_timer_.start();
    time_status_ = MainWindow::STARTED;
    ui->lapButton->setEnabled(true);
}

void MainWindow::pause() {
    ela_msecs_ += elapsed_timer_.elapsed();
    timer_.stop();
    time_status_ = MainWindow::PAUSED;
    ui->lapButton->setEnabled(false);
}

void MainWindow::resume() {
    timer_.start();
    elapsed_timer_.restart();
    time_status_ = MainWindow::STARTED;
    ui->lapButton->setEnabled(true);
}

void MainWindow::stop() {
    timer_.stop();
    ui->lapButton->setEnabled(false);
    ela_msecs_ = 0;
    order_ = 0;
    last_lap_msecs_ = 0;
    model_.removeRows(0, model_.rowCount());
    time_status_ = MainWindow::PREPARED;
}

void MainWindow::lap() {
    order_++;
    QStandardItem *item0 = new QStandardItem(QString::number(order_));

    int msecs = elapsed_timer_.elapsed() + ela_msecs_;
    QTime time = QTime::fromMSecsSinceStartOfDay(msecs);
    QStandardItem *item1 = new QStandardItem(time.toString("mm:ss.zzz"));

    QTime lap_time = QTime::fromMSecsSinceStartOfDay(msecs - last_lap_msecs_);
    QStandardItem *item2 = new QStandardItem(lap_time.toString("mm:ss.zzz"));
    last_lap_msecs_ = msecs;

    QList<QStandardItem *> items{item0, item1, item2};
    model_.insertRow(0, items);
}

void MainWindow::on_startButton_clicked() {
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

void MainWindow::on_stopButton_clicked() {
    stop();
    ui->startButton->setText("Start");
    ui->timeNumber->display("00:00.000");
}

void MainWindow::on_lapButton_clicked() { lap(); }
