#include <attacus/flutter/flutter_view.h>
#include <attacus/flutter/flutter_messenger.h>
#include <attacus/flutter/standard_method_channel.h>
#include <attacus/flutter/components/texture_registrar.h>
#include <examples/example_app.h>

using namespace attacus;

static constexpr char kChannelName[] = "attacus/texture";
static constexpr char kCreateMethod[] = "create";
static constexpr char kDisposeMethod[] = "dispose";
static constexpr char kWidthKey[] = "width";
static constexpr char kHeightKey[] = "height";

class ExampleTexture : public ExampleApp {
public:
    EXAMPLE_CTOR(ExampleTexture)

    virtual void Create() override {
        ExampleApp::Create();

        channel_ = new StandardMethodChannel(messenger(), kChannelName);

        new StandardMethod(*channel_, kCreateMethod,
            [this](const MethodCall<>& call, std::unique_ptr<MethodResult<>> result) {
                //auto args = call.arguments();
                const auto& args = std::get<EncodableMap>(*call.arguments());
                //auto width = std::get<double>(args[0]);
                //auto width = std::get<double>(args.at(EncodableValue("width")));
                auto width_iter = args.find(EncodableValue(std::string(kWidthKey)));
                if (width_iter == args.end()) {
                result->Error("Argument error",
                                "Missing argument while trying to activate system cursor");
                return;
                }
                const auto& width = std::get<double>(width_iter->second);

                //auto height = std::get<double>(args[1]);
                //auto height = std::get<double>(args.at(EncodableValue("height")));
                auto height_iter = args.find(EncodableValue(std::string(kHeightKey)));
                if (height_iter == args.end()) {
                result->Error("Argument error",
                                "Missing argument while trying to activate system cursor");
                return;
                }
                const auto& height = std::get<double>(height_iter->second);
                TextureVariant texture = PixelBufferTexture ([](size_t width, size_t height) ->FlutterDesktopPixelBuffer* {
                    return nullptr;
                });
                auto texId = textureRegistrar().RegisterTexture(&texture);
                textureRegistrar().MarkTextureFrameAvailable(texId);
                result->Success(texId);
            });

        new StandardMethod(*channel_, kDisposeMethod,
            [this](const MethodCall<>& call, std::unique_ptr<MethodResult<>> result) {
                auto args = call.arguments();
                auto id = std::get<int>(args[0]);
                result->Success();
            });

    }
    // Data members
    MethodChannel<>* channel_ = nullptr;
};

EXAMPLE_MAIN(
    ExampleTexture
    , "02-texture"
    , "Load texture."
    , "https://kfields.github.io/attacus/examples.html#texture"
);

