#include "method_channel.h"
#include "json_method_codec.h"
#include "json.h"

namespace attacus
{

typedef MethodResult<JsonValue> JsonMethodResult;
typedef MethodCall<JsonValue> JsonMethodCall;
typedef MethodCallHandler<JsonValue> JsonMethodCallHandler;

class JsonMethodChannel : public MethodChannel<JsonValue>
{
public:
    JsonMethodChannel(FlutterMessenger &messenger, const std::string &name)
        : MethodChannel<JsonValue>(messenger, name, &JsonMethodCodec::GetInstance()) {}
};

} // namespace attacus