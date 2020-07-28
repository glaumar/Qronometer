#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <AbstractExporter.h>

#include <QAction>
#include <QElapsedTimer>
#include <QMainWindow>
#include <QMenu>
#include <QStandardItemModel>
#include <QTime>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   public slots:
    void start();
    void pause();
    void resume();
    void stop();
    void lap();

   private:
    Ui::MainWindow *ui;
    QTimer timer_;
    QElapsedTimer elapsed_timer_;
    enum TimerStatus { PREPARED, STARTED, PAUSED };
    TimerStatus time_status_ = PREPARED;
    qint64 ela_msecs_ = 0;
    qint64 last_lap_msecs_ = 0;
    QStandardItemModel model_{0, 4};
    int order_ = 0;

    void exportData(const AbstractExporter &exporter,
                    const QString &fileFilter = QString());

   private slots:
    void setTimeLabel();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_lapButton_clicked();
};
#endif  // MAINWINDOW_H
