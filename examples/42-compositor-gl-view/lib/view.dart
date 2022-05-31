import 'dart:async';
import 'dart:typed_data';

import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';
import 'package:flutter/rendering.dart';
import 'package:flutter/gestures.dart';
import 'package:flutter/material.dart';

class AttacusViewService {
  static AttacusViewController initAttacusView({
    required int id,
    required String viewType,
    //required TextDirection layoutDirection,
    dynamic creationParams,
    MessageCodec<dynamic>? creationParamsCodec,
    VoidCallback? onFocus,
  }) {
    final AttacusViewController controller = AttacusViewController._(
      viewId: id,
      viewType: viewType,
      //layoutDirection: layoutDirection,
      creationParams: creationParams,
      creationParamsCodec: creationParamsCodec,
    );

    //_instance._focusCallbacks[id] = onFocus ?? () {};
    return controller;
  }
}

class AttacusViewController extends PlatformViewController {
  static const MethodChannel _channel = MethodChannel('attacus/view');

  AttacusViewController._({
    required this.viewId,
    required String viewType,
    //required TextDirection layoutDirection,
    dynamic creationParams,
    MessageCodec<dynamic>? creationParamsCodec,
  })  : _viewType = viewType,
        //_layoutDirection = layoutDirection,
        _creationParams = creationParams,
        _creationParamsCodec = creationParamsCodec;

  @override
  final int viewId;

  final String _viewType;

  final dynamic _creationParams;

  final MessageCodec<dynamic>? _creationParamsCodec;

  @override
  Future<void> create({Size? size}) async {
    await _sendCreateMessage(size: size);
  }

  Future<void> _sendCreateMessage({Size? size}) {
    final Map<String, dynamic> args = <String, dynamic>{
      'id': viewId,
      'viewType': _viewType,
      'width': size?.width,
      'height': size?.height
    };
    if (_creationParams != null) {
      final ByteData paramsByteData =
          _creationParamsCodec!.encodeMessage(_creationParams)!;
      args['params'] = Uint8List.view(
        paramsByteData.buffer,
        0,
        paramsByteData.lengthInBytes,
      );
    }
    return SystemChannels.platform_views.invokeMethod<void>('create', args);
  }

  @override
  Future<void> clearFocus() {
    // TODO: implement clearFocus
    throw UnimplementedError();
  }

  @override
  Future<void> dispatchPointerEvent(PointerEvent event) {
    // TODO: implement dispatchPointerEvent
    throw UnimplementedError();
  }

  @override
  Future<void> dispose() async {
    await _channel.invokeMethod('dispose', {'viewId': viewId});
  }

  /*@override
  int get viewId {
    return _viewId;
  }*/
}

class _AttacusPlatformViewState extends State<AttacusPlatformView> {
  @override
  Widget build(BuildContext context) {
    return PlatformViewLink(
      viewType: 'cubes',
      onCreatePlatformView: _onPlatformViewCreated,
      surfaceFactory:
          (BuildContext context, PlatformViewController controller) {
        return PlatformViewSurface(
          gestureRecognizers: <Factory<OneSequenceGestureRecognizer>>[
            Factory<OneSequenceGestureRecognizer>(
              () => EagerGestureRecognizer(),
            ),
          ].toSet(),
          controller: controller,
          hitTestBehavior: PlatformViewHitTestBehavior.opaque,
        );
      },
    );
  }

  /*var widgetKey = GlobalKey();
  Size oldSize = const Size(0, 0);
  var context = widgetKey.currentContext;*/

  PlatformViewController _onPlatformViewCreated(
      PlatformViewCreationParams params) {
    return AttacusViewService.initAttacusView(
      id: params.id,
      viewType: params.viewType,
    );
  }
}

class AttacusPlatformView extends StatefulWidget {
  const AttacusPlatformView({super.key});

  @override
  State<AttacusPlatformView> createState() => _AttacusPlatformViewState();
}
