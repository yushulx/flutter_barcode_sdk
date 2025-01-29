import 'dart:async';

import 'package:flutter/services.dart';
import 'package:flutter_barcode_sdk/barcode_manager.dart';
import 'package:flutter_web_plugins/flutter_web_plugins.dart';

/// A web implementation of the FlutterBarcodeSdk plugin.
///
/// This class acts as the web interface for barcode scanning, providing
/// barcode decoding capabilities using the [BarcodeManager].
class FlutterBarcodeSdkWeb {
  /// Manages barcode decoding operations.
  final BarcodeManager _barcodeManager = BarcodeManager();

  static void registerWith(Registrar registrar) {
    final MethodChannel channel = MethodChannel(
      'flutter_barcode_sdk',
      const StandardMethodCodec(),
      registrar,
    );

    final pluginInstance = FlutterBarcodeSdkWeb();
    channel.setMethodCallHandler(pluginInstance.handleMethodCall);
  }

  Future<dynamic> handleMethodCall(MethodCall call) async {
    switch (call.method) {
      case 'decodeFile':
        return decodeFile(call.arguments['filename']);
      case 'setBarcodeFormats':
        return setBarcodeFormats(call.arguments['formats']);
      case 'getParameters':
        return getParameters();
      case 'setParameters':
        return setParameters(call.arguments['params']);
      case 'init':
        return initBarcodeSDK();
      case 'setLicense':
        return setLicense(call.arguments['license']);
      case 'decodeImageBuffer':
        return decodeImageBuffer(
            call.arguments['bytes'],
            call.arguments['width'],
            call.arguments['height'],
            call.arguments['stride'],
            call.arguments['format']);
      default:
        throw PlatformException(
          code: 'Unimplemented',
          details:
              'flutter_barcode_sdk for web does not implement method: \'${call.method}\'',
        );
    }
  }

  /// Decodes barcodes from an image file.
  ///
  /// - [file]: The path to the image file.
  ///
  /// Returns a list of detected barcodes.
  Future<List<Map<dynamic, dynamic>>> decodeFile(String file) async {
    return _barcodeManager.decodeFile(file);
  }

  /// Sets the barcode formats to be detected.
  ///
  /// - [formats]: A bitwise OR combination of barcode format constants.
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setBarcodeFormats(int formats) async {
    return _barcodeManager.setBarcodeFormats(formats);
  }

  /// Retrieves the current barcode detection settings as a JSON string.
  ///
  /// Returns a JSON string containing the detection parameters.
  Future<String> getParameters() async {
    return _barcodeManager.getParameters();
  }

  /// Updates the barcode detection parameters.
  ///
  /// - [params]: A JSON string representing the new detection settings.
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setParameters(String params) async {
    return _barcodeManager.setParameters(params);
  }

  /// Initializes the barcode SDK.
  ///
  /// This method must be called before performing any barcode detection operations.
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> initBarcodeSDK() async {
    return await _barcodeManager.initBarcodeSDK();
  }

  /// Sets the license key for the barcode SDK.
  ///
  /// A valid license is required to use barcode detection features.
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setLicense(String license) async {
    return await _barcodeManager.setLicense(license);
  }

  /// Decodes barcodes from an image buffer.
  ///
  /// - [bytes]: The raw image data.
  /// - [width]: The width of the image.
  /// - [height]: The height of the image.
  /// - [stride]: The number of bytes per row.
  /// - [format]: The pixel format (see [ImagePixelFormat]).
  ///
  /// Typically used for **real-time barcode scanning** from a camera stream.
  ///
  /// Returns a list of detected barcodes.
  Future<List<Map<dynamic, dynamic>>> decodeImageBuffer(
      Uint8List bytes, int width, int height, int stride, int format) async {
    return _barcodeManager.decodeImageBuffer(
        bytes, width, height, stride, format);
  }
}
