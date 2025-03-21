#include "collatzthread.h"
#include <QDebug>
#include <chrono>

CollatzThread::CollatzThread(int start, int end, std::atomic<int64_t>* maxSteps, std::atomic<int64_t>* maxPos)
    : startNumber(start), endNumber(end), maxsteps(maxSteps), maxpos(maxPos)
{
}

void CollatzThread::run()
{
    int numberWithMaxChain = startNumber;
    int maxChainLength = 0;

    auto startTime = std::chrono::high_resolution_clock::now();

    for (int number = startNumber; number <= endNumber; ++number) {
        int steps = 1;
        int64_t n = number;

        while (n != 1) {
            if (n % 2 == 0)
                n /= 2;
            else
                n = 3 * n + 1;
            ++steps;
        }

        if (steps > maxChainLength) {
            maxChainLength = steps;
            numberWithMaxChain = number;
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;

    long long timeInMillis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    emit resultReady(numberWithMaxChain, maxChainLength, timeInMillis);
}
