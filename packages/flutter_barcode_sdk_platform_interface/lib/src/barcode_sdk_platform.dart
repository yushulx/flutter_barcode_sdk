import 'package:flutter/services.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'barcode_result.dart';
import 'method_channel_barcode_sdk.dart';

/// A platform interface for the flutter_barcode_sdk plugin.
///
/// Implementations must extend this class and register themselves via
/// [instance] in their `registerWith` method. The default implementation
/// is [MethodChannelBarcodeSdk], which communicates with native code over
/// a [MethodChannel].
///
/// Platform implementations:
/// - Native (Android, iOS, Windows, Linux): use [MethodChannelBarcodeSdk]
///   as the default; native code registers the MethodChannel handler.
/// - Web: overrides [instance] with a Dart-only implementation using
///   JS interop.
abstract class BarcodeSdkPlatform extends PlatformInterface {
  static final Object _token = Object();

  /// The default instance, a [MethodChannelBarcodeSdk].
  static BarcodeSdkPlatform _instance = MethodChannelBarcodeSdk();

  /// The current platform implementation instance.
  static BarcodeSdkPlatform get instance => _instance;

  /// Sets the platform implementation instance.
  ///
  /// Implementations must extend [BarcodeSdkPlatform] to pass the
  /// [PlatformInterface.verify] token check.
  static set instance(BarcodeSdkPlatform instance) {
    PlatformInterface.verify(instance, _token);
    _instance = instance;
  }

  /// Constructs a [BarcodeSdkPlatform].
  BarcodeSdkPlatform() : super(token: _token);

  /// Sets the Dynamsoft Barcode Reader license key.
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setLicense(String license) {
    throw UnimplementedError('setLicense() has not been implemented.');
  }

  /// Initializes the barcode reader.
  ///
  /// Must be called after [setLicense] and before performing any
  /// barcode detection operations.
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> init() {
    throw UnimplementedError('init() has not been implemented.');
  }

  /// Decodes barcodes from an image file.
  ///
  /// [filename] is the path to the image file (or URL on web).
  ///
  /// Returns a list of recognized barcodes.
  Future<List<BarcodeResult>> decodeFile(String filename) {
    throw UnimplementedError('decodeFile() has not been implemented.');
  }

  /// Decodes barcodes from an image buffer.
  ///
  /// - [bytes]: The raw image data.
  /// - [width]: The width of the image.
  /// - [height]: The height of the image.
  /// - [stride]: The stride (number of bytes per row).
  /// - [format]: The pixel format (see [ImagePixelFormat]).
  /// - [rotation]: Rotation angle in degrees (0, 90, 180, 270).
  ///
  /// Typically used for real-time barcode scanning from a camera preview.
  ///
  /// Returns a list of recognized barcodes.
  Future<List<BarcodeResult>> decodeImageBuffer(
      Uint8List bytes,
      int width,
      int height,
      int stride,
      int format,
      int rotation) {
    throw UnimplementedError('decodeImageBuffer() has not been implemented.');
  }

  /// Sets the barcode formats to be detected.
  ///
  /// [formats] is a bitwise combination of barcode format constants
  /// (see [BarcodeFormat]).
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setBarcodeFormats(int formats) {
    throw UnimplementedError('setBarcodeFormats() has not been implemented.');
  }

  /// Retrieves the current barcode detection settings as a JSON string.
  ///
  /// Returns a JSON string containing the detection parameters.
  Future<String> getParameters() {
    throw UnimplementedError('getParameters() has not been implemented.');
  }

  /// Updates the barcode detection parameters.
  ///
  /// [params] is a JSON string representing the new detection settings.
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setParameters(String params) {
    throw UnimplementedError('setParameters() has not been implemented.');
  }
}
