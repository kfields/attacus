#include <examples/example_app.h>

using namespace attacus;

class ExampleHelloWorld : public ExampleApp {
public:
    EXAMPLE_CTOR(ExampleHelloWorld)

    virtual void Create() override {
        ExampleApp::Create();
    }
    //Data members
};

EXAMPLE_MAIN(
    ExampleHelloWorld
    , "00-helloworld"
    , "Initialization."
    , "https://kfields.github.io/attacus/examples.html#helloworld"
);

