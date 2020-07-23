#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QTime>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void start();
    void pause();
    void resume();
    void stop();

private:
    Ui::MainWindow *ui;
    QTimer timer_;
    QElapsedTimer elapsed_timer_;
    enum TimerStatus {PREPARED, STARTED, PAUSED};
    TimerStatus time_status_ = PREPARED;
    qint64 ela_msecs_ = 0;

private slots:
    void setTimeLabel();
    void on_startButton_clicked();
    void on_stopButton_clicked();

};
#endif // MAINWINDOW_H
