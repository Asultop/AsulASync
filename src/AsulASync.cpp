#include "AsulASync.h"
#include <QThreadPool>

AsulASyncRunnable::AsulASyncRunnable(int steps, TaskFunction taskFunc)
    : m_steps(steps)
    , m_currentStep(0)
    , m_taskFunc(std::move(taskFunc))
{
    setAutoDelete(false);
}

void AsulASyncRunnable::run()
{
    if (m_taskFunc) {
        m_taskFunc(this);
    }
    Q_EMIT finished();
}

int AsulASyncRunnable::totalSteps() const
{
    return m_steps;
}

int AsulASyncRunnable::currentStep() const
{
    return m_currentStep;
}

void AsulASyncRunnable::stepNext()
{
    if (m_currentStep < m_steps) {
        ++m_currentStep;
        Q_EMIT stepUpdated(m_currentStep, m_steps);
    }
}
void AsulASyncRunnable::letsStepNext() const
{
    const_cast<AsulASyncRunnable*>(this)->stepNext();
}
void AsulASyncRunnable::start() const
{
    QThreadPool::globalInstance()->start(const_cast<AsulASyncRunnable*>(this));
}
