#include <QCoreApplication>
#include <QThreadPool>
#include <QTest>
#include <QSignalSpy>
#include <QThread>
#include <QAtomicInt>
#include "AsulASync.h"

class TestMultithreading : public QObject
{
    Q_OBJECT

private slots:
    void testConcurrentExecution()
    {
        QAtomicInt counter(0);
        const int taskCount = 5;
        const int stepsPerTask = 10;

        QList<AsulASyncRunnable*> tasks;
        for (int i = 0; i < taskCount; ++i) {
            auto *task = new AsulASyncRunnable(stepsPerTask, [&counter](AsulASyncRunnable *self) {
                for (int j = 0; j < self->totalSteps(); ++j) {
                    counter.fetchAndAddRelaxed(1);
                    self->letsStepNext();
                }
            });
            tasks.append(task);
        }

        for (auto *task : tasks) {
            QThreadPool::globalInstance()->start(task);
        }

        QThreadPool::globalInstance()->waitForDone();
        QCOMPARE(counter.loadAcquire(), taskCount * stepsPerTask);
    }

    void testSignalsFromDifferentThread()
    {
        AsulASyncRunnable *task = new AsulASyncRunnable(1, [](AsulASyncRunnable *self) {
            self->letsStepNext();
        });

        QSignalSpy stepSpy(task, &AsulASyncRunnable::stepUpdated);
        QSignalSpy finishSpy(task, &AsulASyncRunnable::finished);

        QThreadPool::globalInstance()->start(task);
        QThreadPool::globalInstance()->waitForDone();

        QCOMPARE(stepSpy.count(), 1);
        QCOMPARE(finishSpy.count(), 1);
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    TestMultithreading test;
    return QTest::qExec(&test, argc, argv);
}

#include "main.moc"
