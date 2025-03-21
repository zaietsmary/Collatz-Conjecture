#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include <atomic>
#include <shared_mutex>
#include "collatzthread.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonStartClicked();
    void onButtonStopClicked();
    void onSliderValueChanged(int value);
    void onResultReady(int number, int chain, long long time);

private:
    Ui::MainWindow *ui;
    std::atomic<int64_t> maxsteps;
    std::atomic<int64_t> maxpos;
    std::shared_mutex sharedMutex;
};

#endif // MAINWINDOW_H
