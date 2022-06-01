import 'dart:async';
import 'dart:typed_data';

import 'package:flutter/services.dart';
import 'package:flutter/rendering.dart';

enum _AttacusViewState {
  waitingForSize,
  creating,
  created,
  disposed,
}

class AttacusViewController extends PlatformViewController {
  AttacusViewController({
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

  _AttacusViewState _state = _AttacusViewState.waitingForSize;

  /// The texture entry id into which the Android view is rendered.
  int? _textureId;

  /// Returns the texture entry id that the Android view is rendering into.
  ///
  /// Returns null if the Android view has not been successfully created, or if it has been
  /// disposed.
  int? get textureId => _textureId;

  /// The current offset of the platform view.
  Offset _off = Offset.zero;

  @override
  Future<void> create({Size? size}) async {
    assert(_state != _AttacusViewState.disposed,
        'trying to create a disposed Android view');

    await _sendCreateMessage(size: size);

    _state = _AttacusViewState.created;
    /*for (final PlatformViewCreatedCallback callback
        in _platformViewCreatedCallbacks) {
      callback(viewId);
    }*/
  }

  bool get isCreated => _state == _AttacusViewState.created;

  Future<void> _sendCreateMessage({Size? size}) {
    print(viewId);
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
    print('AttacusViewController.dispose');
    if (_state == _AttacusViewState.creating ||
        _state == _AttacusViewState.created) await _sendDisposeMessage();
    //_platformViewCreatedCallbacks.clear();
    _state = _AttacusViewState.disposed;
    //PlatformViewsService._instance._focusCallbacks.remove(viewId);
  }

  Future<void> _sendDisposeMessage() {
    return SystemChannels.platform_views
        .invokeMethod<void>('dispose', <String, dynamic>{'id': viewId});
  }
  /*@override
  int get viewId {
    return _viewId;
  }*/

  Future<Size> setSize(Size size) async {
    assert(_state != _AttacusViewState.disposed,
        'Attacus view is disposed. View id: $viewId');
    assert(_state != _AttacusViewState.waitingForSize,
        'Attacus view must have an initial size. View id: $viewId');
    assert(!size.isEmpty);

    final Map<Object?, Object?>? meta =
        await SystemChannels.platform_views.invokeMapMethod<Object?, Object?>(
      'resize',
      <String, dynamic>{
        'id': viewId,
        'width': size.width,
        'height': size.height,
      },
    );
    assert(meta != null);
    assert(meta!.containsKey('width'));
    assert(meta!.containsKey('height'));
    return Size(meta!['width']! as double, meta['height']! as double);
  }

  Future<void> setOffset(Offset off) async {
    if (off == _off) return;

    // Don't set the offset unless the Android view has been created.
    // The implementation of this method channel throws if the Android view for this viewId
    // isn't addressable.
    if (_state != _AttacusViewState.created) return;

    _off = off;

    await SystemChannels.platform_views.invokeMethod<void>(
      'offset',
      <String, dynamic>{
        'id': viewId,
        'top': off.dy,
        'left': off.dx,
      },
    );
  }
}
