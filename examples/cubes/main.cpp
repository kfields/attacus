#include <examples/example_app.h>
#include <examples/views/cubes/cubes_view.h>

using namespace attacus;

int main(int argc, char **argv)
{
	ExampleApp &app = *ExampleApp::Produce(ExampleParams(
			"cubes",
			"Show cubes.",
			"https://kfields.github.io/attacus/examples.html#cubes"));

	ProduceCubesView(app);
	return app.Run();
}
