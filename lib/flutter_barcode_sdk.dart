import 'dart:async';
import 'dart:typed_data';
import 'package:flutter/services.dart';

class FlutterBarcodeSdk {
  static const int IF_UNKNOWN = -1;
  static const int IF_YUV420 = 0;
  static const int IF_BRGA8888 = 1;

  static const MethodChannel _channel =
      const MethodChannel('flutter_barcode_sdk');

  /// Returns a [String] containing the version of the platform.
  static Future<String> get platformVersion async {
    final String version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }

  List<BarcodeResult> _convertResults(List<Map<dynamic, dynamic>> ret) {
    return ret.map((data) => BarcodeResult.fromJson(data)).toList();
  }

  /// Set Dynamsoft Barcode Reader License Key
  /// Apply for a 30-day FREE trial license: https://www.dynamsoft.com/customer/license/trialLicense
  Future<void> setLicense(String license) async {
    await _channel.invokeMethod('setLicense', {'license': license});
  }

  /// Decodes barcodes from an image file.
  Future<List<BarcodeResult>> decodeFile(String filename) async {
    List<Map<dynamic, dynamic>> ret = List<Map<dynamic, dynamic>>.from(
        await _channel.invokeMethod('decodeFile', {'filename': filename}));
    return _convertResults(ret);
  }

  /// Decodes barcodes from bytes of an image file.
  Future<List<BarcodeResult>> decodeFileBytes(Uint8List bytes) async {
    assert(bytes.isNotEmpty);
    List<Map<dynamic, dynamic>> ret = List<Map<dynamic, dynamic>>.from(
        await _channel.invokeMethod('decodeFileBytes', {'bytes': bytes}));
    return _convertResults(ret);
  }

  /// Decodes barcodes from an image buffer.
  ///
  /// E.g. [CameraImage]
  Future<List<BarcodeResult>> decodeImageBuffer(
      Uint8List bytes, int width, int height, int stride, int format) async {
    List<Map<dynamic, dynamic>> ret = List<Map<dynamic, dynamic>>.from(
        await _channel.invokeMethod('decodeImageBuffer', {
      'bytes': bytes,
      'width': width,
      'height': height,
      'stride': stride,
      'format': format
    }));
    return _convertResults(ret);
  }

  /// Decodes barcodes from webcam stream.
  /// Web only!
  Future<void> decodeVideo() async {
    await _channel.invokeMethod('decodeVideo');
  }
}

/// Barcode result contains barcode format, result, and coordinate points
///
/// https://flutter.dev/docs/development/data-and-backend/json
class BarcodeResult {
  final String format;
  final String text;
  final int x1;
  final int y1;
  final int x2;
  final int y2;
  final int x3;
  final int y3;
  final int x4;
  final int y4;
  final int angle;

  BarcodeResult(this.format, this.text, this.x1, this.y1, this.x2, this.y2,
      this.x3, this.y3, this.x4, this.y4, this.angle);

  BarcodeResult.fromJson(Map<dynamic, dynamic> json)
      : format = json['format'],
        text = json['text'],
        x1 = json['x1'],
        y1 = json['y1'],
        x2 = json['x2'],
        y2 = json['y2'],
        x3 = json['x3'],
        y3 = json['y3'],
        x4 = json['x4'],
        y4 = json['y4'],
        angle = json['angle'];

  Map<String, dynamic> toJson() => {
        'format': format,
        'text': text,
        'x1': x1,
        'y1': y1,
        'x2': x2,
        'y2': y2,
        'x3': x3,
        'y3': y3,
        'x4': x4,
        'y4': y4,
        'angle': angle,
      };
}
