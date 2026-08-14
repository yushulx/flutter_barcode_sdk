/// Stub implementation of [DesktopCameraHelper] for web platforms.
///
/// All methods are no-ops since desktop camera functionality is not
/// available on the web.
class DesktopCameraHelper {
  /// Returns an empty device list (no cameras on web).
  Future<List<String>> getDeviceList() async => [];

  /// Always returns `false` (camera cannot be opened on web).
  Future<bool> open(int index) async => false;

  /// Returns an empty map (no frame data on web).
  Future<Map<String, dynamic>> captureFrame() async => {};

  /// No-op on web.
  Future<void> release() async {}
}
