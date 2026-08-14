import 'package:flutter/services.dart';

import 'barcode_result.dart';
import 'barcode_sdk_platform.dart';

/// Default implementation of [BarcodeSdkPlatform] using a [MethodChannel].
///
/// This is the default instance set on [BarcodeSdkPlatform.instance]. Native
/// platform implementations (Android, iOS, Windows, Linux) rely on this
/// default: their native code registers a handler for the
/// `'flutter_barcode_sdk'` MethodChannel, and this class sends method calls
/// over that channel.
///
/// On web, [BarcodeSdkPlatform.instance] is overridden by the web package's
/// Dart-only implementation that uses JS interop directly.
class MethodChannelBarcodeSdk extends BarcodeSdkPlatform {
  /// The communication channel between Flutter and the native platform.
  static const MethodChannel _channel = MethodChannel('flutter_barcode_sdk');

  @override
  Future<int> setLicense(String license) async {
    return await _channel.invokeMethod('setLicense', {'license': license});
  }

  @override
  Future<int> init() async {
    return await _channel.invokeMethod('init');
  }

  @override
  Future<List<BarcodeResult>> decodeFile(String filename) async {
    final List<Map<dynamic, dynamic>> ret = List<Map<dynamic, dynamic>>.from(
        await _channel.invokeMethod('decodeFile', {'filename': filename}));
    return convertResults(ret);
  }

  @override
  Future<List<BarcodeResult>> decodeImageBuffer(
      Uint8List bytes,
      int width,
      int height,
      int stride,
      int format,
      int rotation) async {
    final List<Map<dynamic, dynamic>> ret = List<Map<dynamic, dynamic>>.from(
        await _channel.invokeMethod('decodeImageBuffer', {
      'bytes': bytes,
      'width': width,
      'height': height,
      'stride': stride,
      'format': format,
      'rotation': rotation
    }));
    return convertResults(ret);
  }

  @override
  Future<int> setBarcodeFormats(int formats) async {
    return await _channel
        .invokeMethod('setBarcodeFormats', {'formats': formats});
  }

  @override
  Future<String> getParameters() async {
    return await _channel.invokeMethod('getParameters');
  }

  @override
  Future<int> setParameters(String params) async {
    return await _channel.invokeMethod('setParameters', {'params': params});
  }
}
