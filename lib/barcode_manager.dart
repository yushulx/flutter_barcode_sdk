@JS('Dynamsoft')
library dynamsoft;

import 'dart:async';
import 'dart:convert';
import 'dart:typed_data';
// import 'dart:js';
import 'package:flutter_barcode_sdk/dynamsoft_barcode.dart';
import 'package:js/js.dart';
import 'utils.dart';

/// BarcodeReader class.
@JS('DBR.BarcodeReader')
class BarcodeReader {
  external static PromiseJsImpl<BarcodeReader> createInstance();
  external PromiseJsImpl<List<dynamic>> decode(dynamic file);
  external PromiseJsImpl<List<dynamic>> decodeBuffer(
      Uint8List bytes, int width, int height, int stride, int format);
  external PromiseJsImpl<dynamic> getRuntimeSettings();
  external PromiseJsImpl<void> updateRuntimeSettings(String settings);
  external PromiseJsImpl<dynamic> outputRuntimeSettingsToString();
  external PromiseJsImpl<void> initRuntimeSettingsWithString(String settings);
  external static set license(String license);
}

/// BarcodeManager class.
class BarcodeManager {
  BarcodeReader? _barcodeReader;

  /// Wrap callback results.
  List<BarcodeResult> callbackResults(List<Map<dynamic, dynamic>> results) {
    return convertResults(results);
  }

  /// Initialize Barcode Reader.
  void initBarcodeReader(BarcodeReader reader) {
    _barcodeReader = reader;
  }

  /// Initialize barcode reader and scanner.
  Future<int> initBarcodeSDK() async {
    _barcodeReader = await handleThenable(BarcodeReader.createInstance());

    return 0;
  }

  /// Set license key.
  Future<int> setLicense(String license) async {
    try {
      BarcodeReader.license = license;
    } catch (e) {
      print(e);
      return -1;
    }

    return 0;
  }

  /// Set barcode formats.
  /// https://www.dynamsoft.com/barcode-reader/parameters/enum/format-enums.html?ver=latest#barcodeformat
  Future<int> setBarcodeFormats(int formats) async {
    updateSettings(formats);
    return 0;
  }

  /// Initialize runtime settings.
  Future<void> updateSettings(int formats) async {
    dynamic settings =
        await handleThenable(_barcodeReader!.getRuntimeSettings());
    Map obj = json.decode(stringify(settings));
    obj['barcodeFormatIds'] = formats;
    await handleThenable(
        _barcodeReader!.updateRuntimeSettings(json.encode(obj)));
  }

  /// Convert List<dynamic> to List<Map<dynamic, dynamic>>.
  List<Map<dynamic, dynamic>> _resultWrapper(List<dynamic> barcodeResults) {
    List<Map<dynamic, dynamic>> results = [];

    for (dynamic result in barcodeResults) {
      Map value = json.decode(stringify(result));

      var tmp = Map<dynamic, dynamic>();
      tmp['format'] = value['barcodeFormatString'];
      // tmp['text'] = value['barcodeText'];
      tmp['x1'] = value['localizationResult']['x1'];
      tmp['y1'] = value['localizationResult']['y1'];
      tmp['x2'] = value['localizationResult']['x2'];
      tmp['y2'] = value['localizationResult']['y2'];
      tmp['x3'] = value['localizationResult']['x3'];
      tmp['y3'] = value['localizationResult']['y3'];
      tmp['x4'] = value['localizationResult']['x4'];
      tmp['y4'] = value['localizationResult']['y4'];
      tmp['angle'] = value['localizationResult']['angle'];
      tmp['barcodeBytes'] = value['barcodeBytes'];
      results.add(tmp);
    }

    return results;
  }

  /// Decode barcodes from an image file.
  Future<List<Map<dynamic, dynamic>>> decodeFile(String filename) async {
    List<dynamic> barcodeResults =
        await handleThenable(_barcodeReader!.decode(filename));

    return _resultWrapper(barcodeResults);
  }

  /// Decode barcodes from an image buffer.
  Future<List<Map<dynamic, dynamic>>> decodeImageBuffer(
      Uint8List bytes, int width, int height, int stride, int format) async {
    List<dynamic> barcodeResults = await handleThenable(
        _barcodeReader!.decodeBuffer(bytes, width, height, stride, format));

    return _resultWrapper(barcodeResults);
  }

  /// Get all current parameters configured for barcode detection algorithm.
  /// https://www.dynamsoft.com/barcode-reader/parameters/reference/image-parameter/?ver=latest
  Future<String> getParameters() async {
    dynamic settings =
        await handleThenable(_barcodeReader!.outputRuntimeSettingsToString());
    return stringify(settings);
  }

  /// Set parameters to adjust barcode detection algorithm.
  Future<int> setParameters(String params) async {
    await handleThenable(_barcodeReader!.initRuntimeSettingsWithString(params));
    return 0;
  }
}
