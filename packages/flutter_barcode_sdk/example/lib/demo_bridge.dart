/// License and analytics bridge for the example app.
///
/// `web/index.html` injects `window.FLUTTER_LITE_CAMERA_LICENSE`; native builds
/// fall back to the trial key compiled into `demo_bridge_stub.dart`.
library;

export 'demo_bridge_stub.dart'
    if (dart.library.js_interop) 'demo_bridge_web.dart';
