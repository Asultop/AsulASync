# AsulASync

AsulASync是一个基于Qt6的异步编程库，旨在简化异步操作的实现和管理。

## 功能特性

- 基于QRunnable的异步任务执行
- 进度跟踪和信号通知
- 支持Lambda和继承两种使用方式
- 与Qt信号槽机制无缝集成

## 构建要求

- Qt 6.7.3
- CMake 3.16+
- MSVC 2022

## 构建步骤

```bash
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

## 使用示例

### Lambda方式

```cpp
auto *task = new AsulASyncRunnable(10, [](AsulASyncRunnable *self) {
    for (int i = 0; i < 10; ++i) {
        // 执行任务
        self->stepNext();
    }
});

connect(task, &AsulASyncRunnable::finished, []() {
    qDebug() << "Task completed!";
});

QThreadPool::globalInstance()->start(task);
```

### 继承方式

```cpp
class MyTask : public AsulASyncRunnable
{
public:
    MyTask() : AsulASyncRunnable(5) {}

    void run() override {
        for (int i = 0; i < totalSteps(); ++i) {
            stepNext();
        }
    }
};
```

## 许可证

MIT License
