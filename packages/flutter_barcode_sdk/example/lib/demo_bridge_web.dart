import 'dart:js_interop';
import 'dart:js_interop_unsafe';

import 'demo_bridge_stub.dart' show kTrialLicenseKey;

/// `window.DemoAnalytics` from `demos/shared/analytics.js`.
///
/// The demo page loads that file, so the Flutter app can report the same GTM
/// events as the hand-written demos. When the page is served on its own (for
/// example with `flutter run -d chrome`) the global is missing and every call
/// below becomes a no-op.
@JS()
extension type _DemoAnalytics._(JSObject _) implements JSObject {
  external void ready(num loadMs);
  external void success(String mode, String inputSource, [JSObject? extra]);
  external void action(String name, [JSObject? extra]);
}

_DemoAnalytics? _analytics() {
  final JSObject? api =
      globalContext.getProperty<JSObject?>('DemoAnalytics'.toJS);
  return api == null ? null : _DemoAnalytics._(api);
}

/// Reads the key the hosting page computed in `web/index.html`.
///
/// The page picks the Codepool license on dynamsoft.com and the trial key
/// anywhere else, so the same build works both hosted and self-hosted.
String resolveDemoLicenseKey() {
  final JSAny? injected =
      globalContext.getProperty<JSAny?>('FLUTTER_LITE_CAMERA_LICENSE'.toJS);
  if (injected.isA<JSString>()) {
    final String key = (injected as JSString).toDart;
    if (key.isNotEmpty) {
      return key;
    }
  }
  return kTrialLicenseKey;
}

void reportDemoReady(int loadMs) {
  _analytics()?.ready(loadMs);
}

void reportDemoScan(String inputSource) {
  _analytics()?.success('camera', inputSource);
}

void reportDemoAction(String name, [Map<String, Object?>? params]) {
  _analytics()?.action(name, params?.jsify() as JSObject?);
}
