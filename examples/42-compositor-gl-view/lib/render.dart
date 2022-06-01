import 'package:flutter/foundation.dart';
import 'package:flutter/rendering.dart';
import 'package:flutter/gestures.dart';
import 'package:flutter/scheduler.dart';

import 'controller.dart';

enum _PlatformViewState {
  uninitialized,
  resizing,
  ready,
}

class RenderAttacusView extends PlatformViewRenderBox {
  /// Creates a render object for an Attacus view.
  RenderAttacusView({
    required AttacusViewController viewController,
    required PlatformViewHitTestBehavior hitTestBehavior,
    required Set<Factory<OneSequenceGestureRecognizer>> gestureRecognizers,
    Clip clipBehavior = Clip.hardEdge,
  })  : _viewController = viewController,
        _clipBehavior = clipBehavior,
        super(
            controller: viewController,
            hitTestBehavior: hitTestBehavior,
            gestureRecognizers: gestureRecognizers) {
    /*_viewController.pointTransformer = (Offset offset) => globalToLocal(offset);
    updateGestureRecognizers(gestureRecognizers);
    _viewController.addOnPlatformViewCreatedListener(_onPlatformViewCreated);*/
    this.hitTestBehavior = hitTestBehavior;
    _setOffset();
  }

  _PlatformViewState _state = _PlatformViewState.uninitialized;

  Size? _currentTextureSize;

  bool _isDisposed = false;

  /// The Attacus view controller for the Attacus view associated with this render object.
  @override
  AttacusViewController get controller => _viewController;

  AttacusViewController _viewController;

  /// Sets a new Attacus view controller.
  @override
  set controller(AttacusViewController controller) {
    if (_viewController == controller) return;
    //_viewController.removeOnPlatformViewCreatedListener(_onPlatformViewCreated);
    super.controller = controller;
    _viewController = controller;
    //_viewController.pointTransformer = (Offset offset) => globalToLocal(offset);
    _sizePlatformView();
    /*if (_viewController.isCreated) {
      markNeedsSemanticsUpdate();
    }
    _viewController.addOnPlatformViewCreatedListener(_onPlatformViewCreated);*/
  }

  /// {@macro flutter.material.Material.clipBehavior}
  ///
  /// Defaults to [Clip.hardEdge], and must not be null.
  Clip get clipBehavior => _clipBehavior;
  Clip _clipBehavior = Clip.hardEdge;
  set clipBehavior(Clip value) {
    if (value != _clipBehavior) {
      _clipBehavior = value;
      markNeedsPaint();
      markNeedsSemanticsUpdate();
    }
  }

  /*void _onPlatformViewCreated(int id) {
    markNeedsSemanticsUpdate();
  }*/

  @override
  bool get sizedByParent => true;

  @override
  bool get alwaysNeedsCompositing => true;

  @override
  bool get isRepaintBoundary => true;

  @override
  void paint(PaintingContext context, Offset offset) {
    print('painting');
    super.paint(context, offset);
  }

  @override
  Size computeDryLayout(BoxConstraints constraints) {
    return constraints.biggest;
  }

  @override
  void performResize() {
    super.performResize();
    _sizePlatformView();
  }

  Future<void> _sizePlatformView() async {
    // Attacus virtual displays cannot have a zero size.
    // Trying to size it to 0 crashes the app, which was happening when starting the app
    // with a locked screen (see: https://github.com/flutter/flutter/issues/20456).
    if (_state == _PlatformViewState.resizing || size.isEmpty) return;

    _state = _PlatformViewState.resizing;
    markNeedsPaint();

    Size targetSize;
    do {
      targetSize = size;
      if (_viewController.isCreated) {
        _currentTextureSize = await _viewController.setSize(targetSize);
      } else {
        await _viewController.create(size: targetSize);
        _currentTextureSize = targetSize;
      }
      // We've resized the platform view to targetSize, but it is possible that
      // while we were resizing the render object's size was changed again.
      // In that case we will resize the platform view again.
    } while (size != targetSize);

    _state = _PlatformViewState.ready;
    markNeedsPaint();
  }

  // Sets the offset of the underlaying platform view on the platform side.
  //
  // This allows the Attacus native view to draw the a11y highlights in the same
  // location on the screen as the platform view widget in the Flutter framework.
  //
  // It also allows platform code to obtain the correct position of the Attacus
  // native view on the screen.
  void _setOffset() {
    SchedulerBinding.instance.addPostFrameCallback((_) async {
      if (!_isDisposed) {
        if (attached) {
          await _viewController.setOffset(localToGlobal(Offset.zero));
          // Schedule a new post frame callback.
          _setOffset();
        }
      }
    });
  }

  final LayerHandle<ClipRectLayer> _clipRectLayer =
      LayerHandle<ClipRectLayer>();

  @override
  void dispose() {
    _isDisposed = true;
    _clipRectLayer.layer = null;
    super.dispose();
  }

  @override
  void describeSemanticsConfiguration(SemanticsConfiguration config) {
    // Don't call the super implementation since `platformViewId` should
    // be set only when the platform view is created, but the concept of
    // a "created" platform view belongs to this subclass.
    config.isSemanticBoundary = true;

    if (_viewController.isCreated) {
      config.platformViewId = _viewController.viewId;
    }
  }
}
