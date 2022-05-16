#pragma once

#include <functional>

#include "flutter_embedder.h"

namespace attacus {

class FlutterWindow;

class FlutterRunner {
public:
    FlutterRunner(FlutterWindow& view);
    void Create();
    // Data members
    FlutterWindow* view_;
    FlutterEngine engine_;
    FlutterTaskRunnerDescription platform_task_runner = {};
    FlutterTaskRunnerDescription render_task_runner = {};
    FlutterCustomTaskRunners custom_task_runners = {};
};

} //namespace attacus