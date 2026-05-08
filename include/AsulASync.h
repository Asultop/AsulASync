#ifndef ASULASYNC_H
#define ASULASYNC_H

#include <QObject>
#include <QRunnable>
#include <functional>

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

    explicit AsulASyncRunnable(int steps, TaskFunction taskFunc = nullptr);
    ~AsulASyncRunnable() override = default;

    void run() override;

    int totalSteps() const;
    int currentStep() const;
    void letsStepNext() const;
    void start() const;
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
};

#endif // ASULASYNC_H
