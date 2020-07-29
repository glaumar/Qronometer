#include "mainwindow.h"

#include <CSVExporter.h>
#include <JsonExporter.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QObject::connect(&timer_, &QTimer::timeout, this,
                     &MainWindow::setTimeLabel);
    timer_.setInterval(11);
    ui->timeNumber->display("00:00.000");
    ui->lapButton->setEnabled(false);

    // model and tableview
    model_.setHorizontalHeaderItem(0, new QStandardItem("#"));
    model_.setHorizontalHeaderItem(1, new QStandardItem("Total"));
    model_.setHorizontalHeaderItem(2, new QStandardItem("Lap"));
    model_.setHorizontalHeaderItem(3, new QStandardItem("Note"));
    ui->tableView->setModel(&model_);
    ui->tableView->setColumnWidth(0, 50);
    ui->tableView->setSortingEnabled(true);

    // export menu
    QMenu *exportMenu = new QMenu(this);
    QAction *csvAction = exportMenu->addAction("CSV document");
    QAction *jsonAction = exportMenu->addAction("JSON document");

    connect(csvAction, &QAction::triggered, [this]() {
        CSVExporter exporter(&model_);
        exportData(exporter, "CSV document (*.csv)");
    });
    connect(jsonAction, &QAction::triggered, [this]() {
        JsonExporter exporter(&model_);
        exportData(exporter, "JSON document (*.json)");
    });

    ui->exportButton->setMenu(exportMenu);
    ui->exportButton->setEnabled(false);
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
    ui->exportButton->setEnabled(true);
}

void MainWindow::resume() {
    timer_.start();
    elapsed_timer_.restart();
    time_status_ = MainWindow::STARTED;
    ui->lapButton->setEnabled(true);
    ui->exportButton->setEnabled(false);
}

void MainWindow::stop() {
    timer_.stop();
    ui->lapButton->setEnabled(false);
    ui->exportButton->setEnabled(false);
    ela_msecs_ = 0;
    order_ = 0;
    last_lap_msecs_ = 0;
    model_.removeRows(0, model_.rowCount());
    time_status_ = MainWindow::PREPARED;
}

void MainWindow::lap() {
    order_++;
    QStandardItem *item0 = new QStandardItem();
    item0->setData(order_, Qt::DisplayRole);
    item0->setEditable(false);

    int msecs = elapsed_timer_.elapsed() + ela_msecs_;
    QTime time = QTime::fromMSecsSinceStartOfDay(msecs);
    QStandardItem *item1 = new QStandardItem(time.toString("mm:ss.zzz"));
    item1->setEditable(false);

    QTime lap_time = QTime::fromMSecsSinceStartOfDay(msecs - last_lap_msecs_);
    QStandardItem *item2 = new QStandardItem(lap_time.toString("mm:ss.zzz"));
    item2->setEditable(false);

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

void MainWindow::exportData(const AbstractExporter &exporter,
                            const QString &fileFilter) {
    QString path = QFileDialog::getSaveFileName(
        this, "Save to...",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        fileFilter);

    exporter.exportTo(path);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (time_status_ != MainWindow::STARTED) {
        return;
    }

    QMessageBox::StandardButton answer = QMessageBox::question(
        this, tr("Quit"), tr("Timer is runing, Are you sure to quit?"));

    if (answer == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}

// TODO:
// - translate to zh
