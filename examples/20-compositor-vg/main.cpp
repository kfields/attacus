#include <attacus/flutter/flutter_composite_view_vg.h>
#include <examples/example_app.h>

using namespace attacus;

int main(int argc, char** argv) {
    ExampleApp& app = *ExampleApp::Produce(ExampleParams(
        "12-compositor",
        "Initialization.",
        "https://kfields.github.io/attacus/examples.html#helloworld"
    ));
    FlutterView& flutter = *FlutterCompositeViewVg::Produce<FlutterCompositeViewVg>(app);

    return app.Run();
}
