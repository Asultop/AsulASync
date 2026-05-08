#include <QCoreApplication>
#include <QThreadPool>
#include <QDebug>
#include "AsulASync.h"

class DownloadTask : public AsulASyncRunnable
{
public:
    explicit DownloadTask(int fileSize)
        : AsulASyncRunnable(fileSize)
    {
    }

    void run() override
    {
        for (int i = 0; i < totalSteps(); ++i) {
            QThread::msleep(100);
            stepNext();
        }
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    auto *task = new DownloadTask(10);

    QObject::connect(task, &AsulASyncRunnable::stepUpdated, [](int current, int total) {
        qDebug() << "Download progress:" << current * 100 / total << "%";
    });

    QObject::connect(task, &AsulASyncRunnable::finished, [&app]() {
        qDebug() << "Download complete!";
        app.quit();
    });

    QObject::connect(task, &AsulASyncRunnable::errorOccurred, [](const QString &error) {
        qDebug() << "Error:" << error;
    });

    QThreadPool::globalInstance()->start(task);

    return app.exec();
}
