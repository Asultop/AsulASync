#include <QCoreApplication>
#include <QThreadPool>
#include <QDebug>
#include "AsulASync.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    auto *task = new AsulASyncRunnable(5, [](AsulASyncRunnable *self) {
        for (int i = 0; i < 5; ++i) {
            qDebug() << "Processing step" << self->currentStep() + 1 << "of" << self->totalSteps();
            self->letsStepNext();
        }
    });

    QObject::connect(task, &AsulASyncRunnable::stepUpdated, [](int current, int total) {
        qDebug() << "Progress:" << current << "/" << total;
    });

    QObject::connect(task, &AsulASyncRunnable::finished, [&app]() {
        qDebug() << "Task finished!";
        app.quit();
    });

    QThreadPool::globalInstance()->start(task);

    return app.exec();
}
