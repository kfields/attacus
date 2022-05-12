#pragma once

#include <attacus/app.h>

namespace attacus {

struct ExampleParams : WindowParams {
    ExampleParams(std::string _name = "Attacus", std::string _description = "An example",
        std::string _url = "https://kfields.github.io/attacus/index.html",
        Point _origin = Point(SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED),
        Size _size = Size(800, 600),
        uint32_t _flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    ) : WindowParams(_name, _origin, _size, _flags) {
        description = _description;
        url = _url;
    }
    std::string description;
    std::string url;
};

class ExampleApp : public App {
public:
    ExampleApp(ExampleParams params = ExampleParams());
    virtual void Create();
    void ShowExampleDialog();

    // Accessors
	const char* getName() const {
		return name_.c_str();
	}
	const char* getDescription() const {
		return description_.c_str();
	}
	const char* getUrl() const {
		return url_.c_str();
	}

    //Data members
    std::string description_;
    std::string url_;
};

} //namespace attacus

#define EXAMPLE_MAIN(_app, ...) \
int main(int argc, char** argv) { \
    _app& app = *new _app(ExampleParams(__VA_ARGS__)); \
    return app.Run(); \
}

#define EXAMPLE_CTOR(_app) \
_app(ExampleParams params = ExampleParams()) \
    : ExampleApp(params) { \
}
