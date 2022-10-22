#include <attacus/flutter/flutter_view.h>
#include <examples/example_app.h>

using namespace attacus;

int main(int argc, char** argv) {
    ExampleApp& app = *ExampleApp::Produce(ExampleParams(
        "compositor-gl",
        "Initialization.",
        "https://kfields.github.io/attacus/examples.html#compositor-gl"
    ));
    FlutterView& flutter = *FlutterView::Produce<FlutterView>(app);

    return app.Run();
}
