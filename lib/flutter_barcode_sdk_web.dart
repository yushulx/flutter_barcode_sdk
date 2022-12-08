import 'dart:async';
// In order to *not* need this ignore, consider extracting the "web" version
// of your plugin as a separate package, instead of inlining it in the same
// package as the core of your plugin.
// ignore: avoid_web_libraries_in_flutter
import 'dart:html' as html show window;
// import 'dart:typed_data';

import 'package:flutter/services.dart';
import 'package:flutter_barcode_sdk/barcode_manager.dart';
import 'package:flutter_web_plugins/flutter_web_plugins.dart';

/// A web implementation of the FlutterBarcodeSdk plugin.
class FlutterBarcodeSdkWeb {
  BarcodeManager _barcodeManager = BarcodeManager();

  static void registerWith(Registrar registrar) {
    final MethodChannel channel = MethodChannel(
      'flutter_barcode_sdk',
      const StandardMethodCodec(),
      registrar,
    );

    final pluginInstance = FlutterBarcodeSdkWeb();
    channel.setMethodCallHandler(pluginInstance.handleMethodCall);
  }

  /// Handles method calls over the MethodChannel of this plugin.
  /// Note: Check the "federated" architecture for a new way of doing this:
  /// https://flutter.dev/go/federated-plugins
  Future<dynamic> handleMethodCall(MethodCall call) async {
    switch (call.method) {
      case 'getPlatformVersion':
        return getPlatformVersion();
      case 'decodeFile':
        return decodeFile(call.arguments['filename']);
      case 'decodeVideo':
        decodeVideo();
        break;
      case 'closeVideo':
        closeVideo();
        break;
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
              'flutter_barcode_sdk for web doesn\'t implement \'${call.method}\'',
        );
    }
  }

  /// Returns a [String] containing the version of the platform.
  Future<String> getPlatformVersion() {
    final version = html.window.navigator.userAgent;
    return Future.value(version);
  }

  /// Decode barcodes from an image file.
  Future<List<Map<dynamic, dynamic>>> decodeFile(String file) async {
    return _barcodeManager.decodeFile(file);
  }

  /// Decode barcodes from real-time video stream.
  Future<void> decodeVideo() async {
    _barcodeManager.decodeVideo();
  }

  /// Close video window.
  Future<void> closeVideo() async {
    _barcodeManager.closeVideo();
  }

  /// Decode barcodes from real-time video stream.
  Future<int> setBarcodeFormats(int formats) async {
    return _barcodeManager.setBarcodeFormats(formats);
  }

  /// Get all current parameters configured for barcode detection algorithm.
  /// https://www.dynamsoft.com/barcode-reader/parameters/reference/image-parameter/?ver=latest
  Future<String> getParameters() async {
    return _barcodeManager.getParameters();
  }

  /// Set parameters to adjust barcode detection algorithm.
  Future<int> setParameters(String params) async {
    return _barcodeManager.setParameters(params);
  }

  /// Initialize barcode reader and scanner.
  Future<int> initBarcodeSDK() async {
    return await _barcodeManager.initBarcodeSDK();
  }

  /// Set license key.
  Future<int> setLicense(String license) async {
    int ret = await _barcodeManager.setLicense(license);
    return ret;
  }

  /// Decode barcodes from an image buffer.
  Future<List<Map<dynamic, dynamic>>> decodeImageBuffer(
      Uint8List bytes, int width, int height, int stride, int format) async {
    return _barcodeManager.decodeImageBuffer(
        bytes, width, height, stride, format);
  }
}
