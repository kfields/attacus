#pragma once

#include <functional>

#include "flutter_embedder.h"

namespace attacus {

class FlutterView;

class FlutterRunner {
public:
    FlutterRunner(FlutterView& view);
    void Create();
    // Data members
    FlutterView* view_;
    FlutterEngine engine_;
    FlutterTaskRunnerDescription platform_task_runner = {};
    FlutterTaskRunnerDescription render_task_runner = {};
    FlutterCustomTaskRunners custom_task_runners = {};
};

} //namespace attacus