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

  /// Captures a single frame. The returned map contains:
  /// - `'data'`: `Uint8List` of RGB pixel data
  /// - `'width'`: image width
  /// - `'height'`: image height
  Future<Map<String, dynamic>> captureFrame() => _plugin.captureFrame();

  /// Releases the camera resources.
  Future<void> release() => _plugin.release();
}
