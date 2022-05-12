#include "method_channel.h"
#include "standard_method_codec.h"

namespace attacus
{

typedef MethodResult<EncodableValue> StandardMethodResult;
typedef MethodCall<EncodableValue> StandardMethodCall;
typedef MethodCallHandler<EncodableValue> StandardMethodCallHandler;

class StandardMethodChannel : public MethodChannel<EncodableValue>
{
public:
    StandardMethodChannel(FlutterMessenger &messenger, const std::string &name)
        : MethodChannel<EncodableValue>(messenger, name, &StandardMethodCodec::GetInstance()) {}
};

} // namespace attacus