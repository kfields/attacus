#include <attacus/app.h>

#include "flutter_runner.h"
#include "flutter_view.h"

namespace attacus {

FlutterRunner::FlutterRunner(FlutterView& view) : view_(&view) {
    platform_task_runner.struct_size = sizeof(FlutterTaskRunnerDescription);
    platform_task_runner.user_data = this;
    platform_task_runner.runs_task_on_current_thread_callback = [](void*)  { return true; };
    platform_task_runner.post_task_callback = [](
        FlutterTask task,
        uint64_t target_time_nanos,
        void* user_data) {
            FlutterRunner& self = *static_cast<FlutterRunner*>(user_data);
            App::PushCallbackEvent(new Delegate([self, task]() -> void {
                FlutterEngineResult result = FlutterEngineRunTask(self.engine_, &task);
            }), &self);
    };

    //
    render_task_runner.struct_size = sizeof(FlutterTaskRunnerDescription);
    render_task_runner.user_data = this;
    render_task_runner.runs_task_on_current_thread_callback = [](void*)  { return true; };
    render_task_runner.post_task_callback = [](
        FlutterTask task,
        uint64_t target_time_nanos,
        void* user_data) {

    };
    //
    custom_task_runners.struct_size = sizeof(FlutterCustomTaskRunners);
    custom_task_runners.platform_task_runner = &platform_task_runner;
    //custom_task_runners.render_task_runner = &render_task_runner;
}

void FlutterRunner::Create() {
    engine_ = view_->engine_;
}

} //namespace attacus