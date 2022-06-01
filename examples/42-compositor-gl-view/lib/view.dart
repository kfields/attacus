import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';
import 'package:flutter/rendering.dart';
import 'package:flutter/gestures.dart';
import 'package:flutter/material.dart';

import 'controller.dart';
import 'render.dart';

class AttacusViewService {
  static AttacusViewController initAttacusView({
    required int id,
    required String viewType,
    //required TextDirection layoutDirection,
    dynamic creationParams,
    MessageCodec<dynamic>? creationParamsCodec,
    VoidCallback? onFocus,
  }) {
    final AttacusViewController controller = AttacusViewController(
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

class AttacusViewSurface extends PlatformViewSurface {
  AttacusViewSurface({
    super.key,
    required AttacusViewController super.controller,
    required super.hitTestBehavior,
    required super.gestureRecognizers,
  }) {
    print('AttacusViewSurface');
  }

  @override
  RenderObject createRenderObject(BuildContext context) {
    print('createRenderObject');
    final AttacusViewController viewController =
        controller as AttacusViewController;
    // Use GL texture based composition.
    // App should use GL texture unless they require to embed a SurfaceView.
    final RenderAttacusView renderBox = RenderAttacusView(
      viewController: viewController,
      gestureRecognizers: gestureRecognizers,
      hitTestBehavior: hitTestBehavior,
    );
    /*viewController.pointTransformer =
        (Offset position) => renderBox.globalToLocal(position);*/
    return renderBox;
  }
}

class _AttacusPlatformViewState extends State<AttacusPlatformView> {
  @override
  Widget build(BuildContext context) {
    return PlatformViewLink(
      viewType: 'cubes',
      onCreatePlatformView: _onPlatformViewCreated,
      surfaceFactory:
          (BuildContext context, PlatformViewController controller) {
        return AttacusViewSurface(
          gestureRecognizers: <Factory<OneSequenceGestureRecognizer>>[
            Factory<OneSequenceGestureRecognizer>(
              () => EagerGestureRecognizer(),
            ),
          ].toSet(),
          controller: controller as AttacusViewController,
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
    var controller = AttacusViewService.initAttacusView(
        //id: params.id, viewType: params.viewType, creationParams: params);
        id: params.id,
        viewType: params.viewType);

    params.onPlatformViewCreated(params.id);
    return controller;
  }
}

class AttacusPlatformView extends StatefulWidget {
  const AttacusPlatformView({super.key});

  @override
  State<AttacusPlatformView> createState() => _AttacusPlatformViewState();
}
