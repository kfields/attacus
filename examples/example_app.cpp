#include "example_app.h"

namespace attacus {

ExampleApp::ExampleApp(ExampleParams params) : App(params),
    description_(params.description), url_(params.url) {
    //debug_ = true;
}

void ExampleApp::Create() {
    App::Create();
}

} //namespace attacus