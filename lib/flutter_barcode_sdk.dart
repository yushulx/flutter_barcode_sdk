import 'dart:async';
import 'dart:typed_data';
import 'package:flutter/services.dart';

class FlutterBarcodeSdk {
  static const int IF_UNKNOWN = -1;
  static const int IF_YUV420 = 0;
  static const int IF_BRGA8888 = 1;

  static const MethodChannel _channel =
      const MethodChannel('flutter_barcode_sdk');

  static Future<String> get platformVersion async {
    final String version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }

  Future<String> decodeFile(String filename) async {
    return await _channel.invokeMethod('decodeFile', {'filename': filename});
  }

  Future<String> decodeFileBytes(Uint8List bytes) async {
    assert(bytes.isNotEmpty);
    return await _channel.invokeMethod('decodeFileBytes', {'bytes': bytes});
  }

  Future<String> decodeImageBuffer(
      Uint8List bytes, int width, int height, int stride, int format) async {
    return await _channel.invokeMethod('decodeImageBuffer', {
      'bytes': bytes,
      'width': width,
      'height': height,
      'stride': stride,
      'format': format
    });
  }
}
