import 'package:flutter_lite_camera/flutter_lite_camera.dart';

/// Desktop camera helper using [FlutterLiteCamera] for Windows and Linux.
///
/// Wraps the `flutter_lite_camera` plugin to provide a consistent API for
/// camera operations on desktop platforms.
class DesktopCameraHelper {
  final FlutterLiteCamera _plugin = FlutterLiteCamera();

  /// Returns all available camera device names.
  Future<List<String>> getDeviceList() => _plugin.getDeviceList();

  /// Opens the camera at the given [index].
  Future<bool> open(int index) => _plugin.open(index);

  /// Starts the native preview stream and returns a texture id that can be
  /// displayed with a [Texture] widget. Call [open] first.
  Future<int> startPreview() => _plugin.startPreview();

  /// Stops the preview stream started with [startPreview].
  Future<void> stopPreview() => _plugin.stopPreview();

  /// Captures a single frame. The returned map contains:
  /// - `'data'`: `Uint8List` of RGB pixel data
  /// - `'width'`: image width
  /// - `'height'`: image height
  ///
  /// While a preview is running, the frame comes from a native cache and the
  /// preview stream is unaffected.
  Future<Map<String, dynamic>> captureFrame() => _plugin.captureFrame();

  /// Releases the camera resources.
  Future<void> release() => _plugin.release();
}
