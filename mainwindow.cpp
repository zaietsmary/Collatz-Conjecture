#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "collatzthread.h"
#include <QThreadPool>
#include <QTextEdit>
#include <QDebug>
#include <shared_mutex>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), maxsteps(0), maxpos(0)
{
    ui->setupUi(this);

    connect(ui->buttonStart, &QPushButton::clicked, this, &MainWindow::onButtonStartClicked);
    connect(ui->buttonStop, &QPushButton::clicked, this, &MainWindow::onButtonStopClicked);
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::onSliderValueChanged);
    connect(ui->buttonExit, &QPushButton::clicked, this, &QWidget::close);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButtonStartClicked()
{
    ui->result->clear();
    ui->buttonStart->setEnabled(false);
    ui->buttonStop->setEnabled(true);
    ui->spinBox->setEnabled(false);
    ui->horizontalSlider->setEnabled(false);
    int64_t maxNumber = ui->spinBox->value();

    const uint64_t maxLimit = UINT64_MAX;
    if (maxNumber > maxLimit) {
        ui->result->setText(QString("Maximum number cannot exceed %1").arg(maxLimit));
        return;
    }

    int numThreads = ui->horizontalSlider->value();

    int rangePerThread = maxNumber / numThreads;
    int remainingRange = maxNumber % numThreads;

    int start = 1;
    for (int i = 0; i < numThreads; ++i) {
        int end = start + rangePerThread - 1;
        if (i < remainingRange) {
            end++;
        }

        std::unique_lock<std::shared_mutex> lock(sharedMutex);
        CollatzThread *task = new CollatzThread(start, end, &maxsteps, &maxpos);
        task->setAutoDelete(true);
        connect(task, &CollatzThread::resultReady, this, &MainWindow::onResultReady);
        QThreadPool::globalInstance()->start(task);

        start = end + 1;
    }
}

void MainWindow::onButtonStopClicked()
{
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setEnabled(false);
    ui->spinBox->setEnabled(true);
    ui->horizontalSlider->setEnabled(true);
    ui->result->setText(QString("The program was stopped."));
}

void MainWindow::onSliderValueChanged(int value)
{
    ui->sliderValue->setText(QString::number(value));
}

void MainWindow::onResultReady(int number, int chain, long long time)
{
    {
        std::unique_lock<std::shared_mutex> lock(sharedMutex);
        if (chain > maxsteps.load()) {
            maxsteps.store(chain);
            maxpos.store(number);
        }
    }

    {
        std::shared_lock<std::shared_mutex> lock(sharedMutex);
        ui->result->setText(QString("Number with the largest chain: %1 with %2 steps\nTime: %3 milliseconds")
                                .arg(maxpos.load())
                                .arg(maxsteps.load())
                                .arg(time));
    }
}
