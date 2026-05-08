#include <QCoreApplication>
#include <QThreadPool>
#include <QTest>
#include <QSignalSpy>
#include "AsulASync.h"

class TestBasicFunctionality : public QObject
{
    Q_OBJECT

private slots:
    void testStepProgress()
    {
        AsulASyncRunnable task(3, nullptr);
        QSignalSpy spy(&task, &AsulASyncRunnable::stepUpdated);

        task.letsStepNext();
        task.letsStepNext();
        task.letsStepNext();

        QCOMPARE(spy.count(), 3);
        QCOMPARE(task.currentStep(), 3);
        QCOMPARE(task.totalSteps(), 3);
    }

    void testLambdaExecution()
    {
        bool executed = false;
        AsulASyncRunnable task(1, [&executed](AsulASyncRunnable *self) {
            executed = true;
            self->letsStepNext();
        });

        task.run();
        QVERIFY(executed);
        QCOMPARE(task.currentStep(), 1);
    }

    void testFinishedSignal()
    {
        AsulASyncRunnable task(1, [](AsulASyncRunnable *self) {
            self->letsStepNext();
        });

        QSignalSpy spy(&task, &AsulASyncRunnable::finished);
        task.run();

        QCOMPARE(spy.count(), 1);
    }
};

int main(int argc, char *argv[])
{
    TestBasicFunctionality test;
    return QTest::qExec(&test, argc, argv);
}

#include "main.moc"
