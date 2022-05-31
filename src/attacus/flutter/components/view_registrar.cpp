#include <iostream>

#include <attacus/flutter/standard_method_channel.h>

#include <attacus/flutter/flutter_view.h>

#include "view_registrar.h"

namespace attacus
{
static constexpr char kChannelName[] = "flutter/platform_views";
static constexpr char kCreateMethod[] = "create";
static constexpr char kDisposeMethod[] = "dispose";

static constexpr char kIdKey[] = "id";
static constexpr char kViewTypeKey[] = "viewType";
static constexpr char kWidthKey[] = "width";
static constexpr char kHeightKey[] = "height";

ViewRegistrar::ViewRegistrar(FlutterView& view) : FlutterComponent(view) {}

ViewRegistrar::~ViewRegistrar() = default;

void ViewRegistrar::Create()
{
    FlutterMessenger &messenger = flutter().messenger();
    channel_ = new StandardMethodChannel(messenger, kChannelName);

    new StandardMethod(*channel_, kCreateMethod,
        [this](const MethodCall<>& call, std::unique_ptr<MethodResult<>> result) {
            const auto& args = std::get<EncodableMap>(*call.arguments());

            auto id_iter = args.find(EncodableValue(std::string(kIdKey)));
            if (id_iter == args.end()) {
                result->Error("Argument error",
                                "Missing view id");
                return;
            }
            const uint16_t id = std::get<int>(id_iter->second);


            auto type_iter = args.find(EncodableValue(std::string(kViewTypeKey)));
            if (type_iter == args.end()) {
                result->Error("Argument error",
                                "Missing view type");
                return;
            }
            const std::string viewType = std::get<std::string>(type_iter->second);

            uint16_t width = 400;
            auto width_iter = args.find(EncodableValue(std::string(kWidthKey)));
            if (width_iter != args.end()) {
                width = std::get<double>(width_iter->second);
            }

            uint16_t height = 400;
            auto height_iter = args.find(EncodableValue(std::string(kHeightKey)));
            if (height_iter != args.end()) {
                height = std::get<double>(height_iter->second);
            }

            /*cubes_view_ = ExampleCubesView::Produce<ExampleCubesView>(*this, ViewParams(Size(width, height)));
            auto id = viewRegistrar().RegisterView(*cubes_view_);*/

            result->Success();
        });

    new StandardMethod(*channel_, kDisposeMethod,
        [this](const MethodCall<>& call, std::unique_ptr<MethodResult<>> result) {
            auto args = call.arguments();
            auto id = std::get<int>(args[0]);
            result->Success();
        });

}

int64_t ViewRegistrar::RegisterView(View& view)
{
    auto id = view.id();
    views_[id] = &view;
    return id;
}

bool ViewRegistrar::UnregisterView(int64_t id)
{
    return true;
}

} // namespace attacus