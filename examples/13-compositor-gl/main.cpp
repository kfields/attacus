#include <attacus/flutter/flutter_composite_view_gl.h>
#include <examples/example_app.h>

using namespace attacus;

int main(int argc, char** argv) {
    ExampleApp& app = *ExampleApp::Produce(ExampleParams(
        "13-compositor-gl",
        "Initialization.",
        "https://kfields.github.io/attacus/examples.html#helloworld"
    ));
    FlutterView& flutter = *FlutterCompositeViewGL::Produce<FlutterCompositeViewGL>(app);

    return app.Run();
}
