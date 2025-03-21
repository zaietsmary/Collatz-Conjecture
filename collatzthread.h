#ifndef COLLATZTHREAD_H
#define COLLATZTHREAD_H

#include <QRunnable>
#include <atomic>
#include <QObject>

class CollatzThread : public QObject, public QRunnable
{
    Q_OBJECT

public:
    CollatzThread(int start, int end, std::atomic<int64_t>* maxSteps, std::atomic<int64_t>* maxPos);
    void run() override;

signals:
void resultReady(int numberWithMaxChain, int maxChainLength, double timeTaken);

private:
    int startNumber;
    int endNumber;
    std::atomic<int64_t>* maxsteps;
    std::atomic<int64_t>* maxpos;
};

#endif // COLLATZTHREAD_H
