import 'dart:typed_data';

import 'package:flutter_barcode_sdk_platform_interface/flutter_barcode_sdk_platform_interface.dart';

import 'src/template.dart';

export 'package:flutter_barcode_sdk_platform_interface/flutter_barcode_sdk_platform_interface.dart'
    show
        BarcodeResult,
        BarcodeFormat,
        ImagePixelFormat,
        ImageRotation,
        convertResults;

/// A Flutter plugin for barcode scanning using the Dynamsoft Barcode
/// Reader SDK.
///
/// Provides methods for initializing the SDK, configuring barcode detection
/// parameters, and decoding barcodes from image files and raw pixel buffers.
///
/// This is the app-facing package of the federated plugin. All method calls
/// are delegated to [BarcodeSdkPlatform.instance], which is set by the
/// endorsed platform implementation packages:
///
/// - Android: `flutter_barcode_sdk_android`
/// - iOS: `flutter_barcode_sdk_ios`
/// - Windows: `flutter_barcode_sdk_windows`
/// - Linux: `flutter_barcode_sdk_linux`
/// - Web: `flutter_barcode_sdk_web`
///
/// ### Quick Start
/// ```dart
/// final reader = FlutterBarcodeSdk();
/// await reader.setLicense('YOUR-LICENSE-KEY');
/// await reader.init();
/// List<BarcodeResult> results = await reader.decodeFile('path/to/image.png');
/// ```
///
/// Supported platforms: Android, iOS, Web, Windows, Linux.
class FlutterBarcodeSdk {
  /// Sets the Dynamsoft Barcode Reader license key.
  ///
  /// A valid license is required to use the barcode scanning functionality.
  /// You can apply for a **30-day FREE trial license** at:
  /// [Dynamsoft License Portal](https://www.dynamsoft.com/customer/license/trialLicense/?product=dcv&package=cross-platform).
  ///
  /// - [license]: The license key.
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setLicense(String license) {
    return BarcodeSdkPlatform.instance.setLicense(license);
  }

  /// Decodes barcodes from an image file.
  ///
  /// - [filename]: The path to the image file.
  ///
  /// Returns a list of recognized barcodes.
  Future<List<BarcodeResult>> decodeFile(String filename) {
    return BarcodeSdkPlatform.instance.decodeFile(filename);
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
  /// Typically used for **real-time barcode scanning** from a camera preview.
  ///
  /// Returns a list of recognized barcodes.
  Future<List<BarcodeResult>> decodeImageBuffer(Uint8List bytes, int width,
      int height, int stride, int format, int rotation) {
    return BarcodeSdkPlatform.instance.decodeImageBuffer(
        bytes, width, height, stride, format, rotation);
  }

  /// Sets the barcode formats to be detected.
  ///
  /// - [formats]: A bitwise combination of barcode formats (see
  ///   [BarcodeFormat]). On web, values are automatically converted to
  ///   JavaScript `BigInt` to support the full 64-bit range.
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setBarcodeFormats(int formats) {
    return BarcodeSdkPlatform.instance.setBarcodeFormats(formats);
  }

  /// Retrieves the current barcode detection settings as a JSON string.
  ///
  /// Returns a JSON string containing the detection parameters.
  Future<String> getParameters() {
    return BarcodeSdkPlatform.instance.getParameters();
  }

  /// Updates the barcode detection parameters.
  ///
  /// - [params]: A JSON string representing the new detection settings.
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setParameters(String params) {
    return BarcodeSdkPlatform.instance.setParameters(params);
  }

  /// Initializes the barcode reader and applies default detection
  /// parameters.
  ///
  /// Must be called **after** [setLicense] and **before** performing any
  /// barcode detection operations.
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> init() async {
    int ret = await BarcodeSdkPlatform.instance.init();
    ret = await setParameters(template);
    return ret;
  }
}
