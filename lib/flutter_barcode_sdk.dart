import 'dart:async';
import 'package:flutter/services.dart';
import 'package:flutter_barcode_sdk/dynamsoft_barcode.dart';

/// Defines the supported image pixel formats.
enum ImagePixelFormat {
  /// Binary format (0: Black, 1: White).
  IPF_BINARY,

  /// Inverted binary format (0: White, 1: Black).
  IPF_BINARYINVERTED,

  /// 8-bit grayscale format.
  IPF_GRAYSCALED,

  /// NV21 format (commonly used in Android camera preview).
  IPF_NV21,

  /// 16-bit RGB format with 5 bits for red, 6 bits for green, and 5 bits for blue.
  IPF_RGB_565,

  /// 16-bit RGB format with 5 bits for each color channel.
  IPF_RGB_555,

  /// 24-bit RGB format with 8 bits for each color channel.
  IPF_RGB_888,

  /// 32-bit ARGB format (Alpha, Red, Green, Blue) stored from high to low memory address.
  IPF_ARGB_8888,

  /// 48-bit RGB format with 16 bits for each color channel.
  IPF_RGB_161616,

  /// 64-bit ARGB format with 16 bits for each channel.
  IPF_ARGB_16161616,

  /// 32-bit ABGR format (Alpha, Blue, Green, Red) stored from high to low memory address.
  IPF_ABGR_8888,

  /// 64-bit ABGR format with 16 bits for each channel.
  IPF_ABGR_16161616,

  /// 24-bit BGR format with 8 bits for each channel.
  IPF_BGR_888,
}

/// A Flutter plugin for barcode scanning using the Dynamsoft Barcode Reader SDK.
///
/// This class provides methods for setting up the SDK, configuring barcode formats,
/// and decoding barcodes from images and raw pixel data.
class FlutterBarcodeSdk {
  /// The communication channel between Flutter and the native platform.
  static const MethodChannel _channel = MethodChannel('flutter_barcode_sdk');

  /// Sets the Dynamsoft Barcode Reader license key.
  ///
  /// A valid license is required to use the barcode scanning functionality.
  /// You can apply for a **30-day FREE trial license** at:
  /// [Dynamsoft License Portal](https://www.dynamsoft.com/customer/license/trialLicense/?product=dcv&package=cross-platform).
  ///
  /// - [license]: The license key.
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setLicense(String license) async {
    return await _channel.invokeMethod('setLicense', {'license': license});
  }

  /// Decodes barcodes from an image file.
  ///
  /// - [filename]: The path to the image file.
  ///
  /// Returns a list of recognized barcodes.
  Future<List<BarcodeResult>> decodeFile(String filename) async {
    final List<Map<dynamic, dynamic>> ret = List<Map<dynamic, dynamic>>.from(
        await _channel.invokeMethod('decodeFile', {'filename': filename}));
    return convertResults(ret);
  }

  /// Decodes barcodes from an image buffer.
  ///
  /// - [bytes]: The raw image data.
  /// - [width]: The width of the image.
  /// - [height]: The height of the image.
  /// - [stride]: The stride (number of bytes per row).
  /// - [format]: The pixel format (see [ImagePixelFormat]).
  ///
  /// Typically used for **real-time barcode scanning** from a camera preview.
  ///
  /// Returns a list of recognized barcodes.
  Future<List<BarcodeResult>> decodeImageBuffer(
      Uint8List bytes, int width, int height, int stride, int format) async {
    final List<Map<dynamic, dynamic>> ret = List<Map<dynamic, dynamic>>.from(
        await _channel.invokeMethod('decodeImageBuffer', {
      'bytes': bytes,
      'width': width,
      'height': height,
      'stride': stride,
      'format': format
    }));
    return convertResults(ret);
  }

  /// Sets the barcode formats to be detected.
  ///
  /// - [formats]: A bitwise combination of barcode formats (see [BarcodeFormat]).
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setBarcodeFormats(int formats) async {
    return await _channel
        .invokeMethod('setBarcodeFormats', {'formats': formats});
  }

  /// Retrieves the current barcode detection settings as a JSON string.
  ///
  /// Returns a JSON string containing the detection parameters.
  Future<String> getParameters() async {
    return await _channel.invokeMethod('getParameters');
  }

  /// Updates the barcode detection parameters.
  ///
  /// - [params]: A JSON string representing the new detection settings.
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setParameters(String params) async {
    return await _channel.invokeMethod('setParameters', {'params': params});
  }

  /// Initializes the barcode reader.
  ///
  /// Must be called before performing barcode detection.
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> init() async {
    return await _channel.invokeMethod('init');
  }
}
