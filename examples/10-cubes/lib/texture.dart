import 'package:flutter/services.dart';

class OpenGLTextureController {
  static const MethodChannel _channel = MethodChannel('attacus/texture');

  int textureId = -1;

  Future<int> initialize(double width, double height) async {
    textureId = await _channel.invokeMethod('create', {
      'width': width,
      'height': height,
    });
    return textureId;
  }

  Future<void> dispose() =>
      _channel.invokeMethod('dispose', {'textureId': textureId});

  bool get isInitialized => textureId != -1;
}
