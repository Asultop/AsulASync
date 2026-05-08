#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QThreadPool>
#include <QLabel>
#include <QTimer>
#include <QSpinBox>
#include <QSlider>
#include <QLineEdit>
#include <QElapsedTimer>
#include "AsulASync.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("AsulASync GUI Example");
    window.resize(500, 400);

    auto *mainLayout = new QVBoxLayout(&window);

    // --- Task section ---
    mainLayout->addWidget(new QLabel("=== Async Task ==="));

    auto *taskLabel = new QLabel("Click the button to start task");
    mainLayout->addWidget(taskLabel);

    auto *progressBar = new QProgressBar;
    progressBar->setRange(0, 100);
    mainLayout->addWidget(progressBar);

    auto *elapsedLabel = new QLabel("Elapsed: 0 ms");
    mainLayout->addWidget(elapsedLabel);

    auto *button = new QPushButton("Start Task");
    mainLayout->addWidget(button);

    // --- UI responsiveness section ---
    mainLayout->addWidget(new QLabel("=== UI Responsiveness (try while task runs) ==="));

    // Elapsed timer counting in real-time to prove event loop is alive
    auto *tickLabel = new QLabel("UI Tick: 0 ms");
    mainLayout->addWidget(tickLabel);

    QElapsedTimer uiTick;
    uiTick.start();
    auto *tickTimer = new QTimer(&window);
    QObject::connect(tickTimer, &QTimer::timeout, [&]() {
        tickLabel->setText(QString("UI Tick: %1 ms").arg(uiTick.elapsed()));
    });
    tickTimer->start(16); // ~60fps

    // SpinBox
    auto *spinLayout = new QHBoxLayout;
    spinLayout->addWidget(new QLabel("SpinBox:"));
    auto *spinBox = new QSpinBox;
    spinBox->setRange(0, 9999);
    spinLayout->addWidget(spinBox);
    mainLayout->addLayout(spinLayout);

    // Slider linked to SpinBox
    auto *sliderLayout = new QHBoxLayout;
    sliderLayout->addWidget(new QLabel("Slider:"));
    auto *slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 9999);
    sliderLayout->addWidget(slider);
    mainLayout->addLayout(sliderLayout);

    QObject::connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), slider, &QSlider::setValue);
    QObject::connect(slider, &QSlider::valueChanged, spinBox, &QSpinBox::setValue);

    // LineEdit with live echo
    mainLayout->addWidget(new QLabel("Type here (should be responsive during task):"));
    auto *lineEdit = new QLineEdit;
    mainLayout->addWidget(lineEdit);
    auto *echoLabel = new QLabel;
    mainLayout->addWidget(echoLabel);
    QObject::connect(lineEdit, &QLineEdit::textChanged, echoLabel, &QLabel::setText);

    // Click counter
    auto *clickButton = new QPushButton("Click me! (count: 0)");
    mainLayout->addWidget(clickButton);
    int clickCount = 0;
    QObject::connect(clickButton, &QPushButton::clicked, [&]() {
        ++clickCount;
        clickButton->setText(QString("Click me! (count: %1)").arg(clickCount));
    });

    // --- Task logic ---
    QElapsedTimer taskTimer;

    QObject::connect(button, &QPushButton::clicked, [&]() {
        button->setEnabled(false);
        progressBar->setValue(0);
        taskLabel->setText("Running...");
        taskTimer.start();

        auto *task = new AsulASyncRunnable(100, [](AsulASyncRunnable *self) {
            for (int i = 0; i < self->totalSteps(); ++i) {
                QThread::msleep(30);
                self->letsStepNext();
            }
        }, &window);

        QObject::connect(task, &AsulASyncRunnable::stepUpdated,
                         progressBar, [progressBar](int current, int total) {
            progressBar->setValue(current * 100 / total);
        });

        QObject::connect(task, &AsulASyncRunnable::finished, [&]() {
            taskLabel->setText(QString("Task finished! (%1 ms)").arg(taskTimer.elapsed()));
            button->setEnabled(true);
        });

        task->start();
    });

    window.show();
    return app.exec();
}
