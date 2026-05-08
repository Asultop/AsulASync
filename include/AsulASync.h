#ifndef ASULASYNC_H
#define ASULASYNC_H

#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <QWaitCondition>
#include <functional>
#include <atomic>

#if defined(ASULASYNC_LIBRARY)
#  define ASULASYNC_EXPORT Q_DECL_EXPORT
#else
#  define ASULASYNC_EXPORT Q_DECL_IMPORT
#endif

class ASULASYNC_EXPORT AsulASyncRunnable : public QObject, public QRunnable
{
    Q_OBJECT

public:
    using TaskFunction = std::function<void(AsulASyncRunnable*)>;

    explicit AsulASyncRunnable(int steps, TaskFunction taskFunc = nullptr, QObject *parent = nullptr);
    explicit AsulASyncRunnable(int steps, QObject *parent = nullptr);
    explicit AsulASyncRunnable();
    
    ~AsulASyncRunnable() override;

    void run() override;

    int totalSteps() const;
    int currentStep() const;
    void letsStepNext() const;
    void start() const;
    void cancel();
    bool isCancelled() const;
signals:
    void finished();
    void errorOccurred(const QString& error);
    void stepUpdated(int currentStep, int totalSteps);

protected:
    void stepNext();

private:
    int m_steps;
    int m_currentStep;
    TaskFunction m_taskFunc;
    std::atomic<bool> m_cancelled;
};

#endif // ASULASYNC_H
