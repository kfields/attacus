#include <attacus/flutter/flutter_view.h>
#include <attacus/flutter/flutter_messenger.h>
#include <attacus/flutter/standard_method_channel.h>

#include <examples/example_app.h>

using namespace attacus;

class ExampleMethodChannel : public FlutterView {
public:
    ExampleMethodChannel(View& parent, ViewParams params = ViewParams()) : FlutterView(parent, params) {}

    int counter_ = 0;

    virtual void Create() override {
        FlutterView::Create();

        channel_ = new StandardMethodChannel(messenger(), "example");

        new StandardMethod(*channel_, "incrementCounter",
            [this](const MethodCall<>& call, std::unique_ptr<MethodResult<>> result) {
                counter_++;
                result->Success(counter_);
            });

        new StandardMethod(*channel_, "setValue",
            [this](const MethodCall<>& call, std::unique_ptr<MethodResult<>> result) {
                result->Success();
            });
    }

    // Data members
    MethodChannel<>* channel_ = nullptr;
};

int main(int argc, char** argv) {
    ExampleApp& app = *ExampleApp::Produce(ExampleParams(
        "00-methodchannel",
        "Test method channels.",
        "https://kfields.github.io/attacus/examples.html#methodchannel"
    ));
    FlutterView& flutter = *ExampleMethodChannel::Produce<ExampleMethodChannel>(app);
    return app.Run();
}
