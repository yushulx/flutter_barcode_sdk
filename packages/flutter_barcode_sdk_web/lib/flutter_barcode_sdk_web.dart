import 'dart:typed_data';

import 'package:flutter/services.dart';
import 'package:flutter_barcode_sdk_platform_interface/flutter_barcode_sdk_platform_interface.dart';
import 'package:flutter_web_plugins/flutter_web_plugins.dart';

import 'src/barcode_manager.dart';

/// Web implementation of [BarcodeSdkPlatform].
///
/// This class uses the Dynamsoft Barcode Reader JS SDK via `dart:js_interop`
/// to perform barcode decoding directly in Dart, bypassing the MethodChannel
/// used by native platform implementations.
///
/// Registration happens via [registerWith], which sets this class as the
/// [BarcodeSdkPlatform.instance] on web.
class FlutterBarcodeSdkWeb extends BarcodeSdkPlatform {
  /// Manages barcode decoding operations using the JS SDK.
  final BarcodeManager _barcodeManager = BarcodeManager();

  /// Registers this implementation as the [BarcodeSdkPlatform.instance]
  /// on the web platform.
  static void registerWith(Registrar registrar) {
    BarcodeSdkPlatform.instance = FlutterBarcodeSdkWeb();
  }

  @override
  Future<int> setLicense(String license) async {
    return _barcodeManager.setLicense(license);
  }

  @override
  Future<int> init() async {
    return _barcodeManager.initBarcodeSDK();
  }

  @override
  Future<List<BarcodeResult>> decodeFile(String filename) async {
    final results = await _barcodeManager.decodeFile(filename);
    return convertResults(results);
  }

  @override
  Future<List<BarcodeResult>> decodeImageBuffer(
      Uint8List bytes,
      int width,
      int height,
      int stride,
      int format,
      int rotation) async {
    final results = await _barcodeManager.decodeImageBuffer(
        bytes, width, height, stride, format, rotation);
    return convertResults(results);
  }

  @override
  Future<int> setBarcodeFormats(int formats) async {
    return _barcodeManager.setBarcodeFormats(formats);
  }

  @override
  Future<String> getParameters() async {
    return _barcodeManager.getParameters();
  }

  @override
  Future<int> setParameters(String params) async {
    return _barcodeManager.setParameters(params);
  }
}
