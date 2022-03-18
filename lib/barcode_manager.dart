@JS('Dynamsoft')
library dynamsoft;

import 'dart:async';
import 'dart:convert';
// import 'dart:js';
import 'package:flutter_barcode_sdk/dynamsoft_barcode.dart';
import 'package:flutter_barcode_sdk/global.dart';
import 'package:js/js.dart';
import 'utils.dart';

// @JS('DBR')
// class DBR {
//   external static Object get EnumBarcodeFormat;
// }

/// BarcodeScanner class.
@JS('DBR.BarcodeScanner')
class BarcodeScanner {
  external static PromiseJsImpl<BarcodeScanner> createInstance();
  external void show();
  external void hide();
  external set onFrameRead(Function func);
  external PromiseJsImpl<dynamic> getRuntimeSettings();
  external PromiseJsImpl<void> updateRuntimeSettings(String settings);
  external PromiseJsImpl<dynamic> outputSettingsToString();
  external PromiseJsImpl<void> initRuntimeSettingsWithString(String settings);
}

/// BarcodeReader class.
@JS('DBR.BarcodeReader')
class BarcodeReader {
  external static PromiseJsImpl<BarcodeReader> createInstance();
  external PromiseJsImpl<List<dynamic>> decode(dynamic file);
  external PromiseJsImpl<dynamic> getRuntimeSettings();
  external PromiseJsImpl<void> updateRuntimeSettings(String settings);
  external PromiseJsImpl<dynamic> outputSettingsToString();
  external PromiseJsImpl<void> initRuntimeSettingsWithString(String settings);
}

/// BarcodeManager class.
class BarcodeManager {
  BarcodeScanner? _barcodeScanner;
  BarcodeReader? _barcodeReader;

  /// Wrap callback results.
  List<BarcodeResult> callbackResults(List<Map<dynamic, dynamic>> results) {
    return convertResults(results);
  }

  /// Initialize Barcode Scanner.
  void initBarcodeScanner(BarcodeScanner scanner) {
    _barcodeScanner = scanner;
    _barcodeScanner!.onFrameRead = allowInterop((results) =>
        {globalCallback(callbackResults(_resultWrapper(results)))});
  }

  /// Initialize Barcode Reader.
  void initBarcodeReader(BarcodeReader reader) {
    _barcodeReader = reader;
  }

  /// Initialize barcode reader and scanner.
  Future<void> initBarcodeSDK() async {
    BarcodeReader reader = await handleThenable(BarcodeReader.createInstance());
    initBarcodeReader(reader);

    BarcodeScanner scanner =
        await handleThenable(BarcodeScanner.createInstance());
    initBarcodeScanner(scanner);
  }

  /// Show camera view.
  Future<void> decodeVideo() async {
    _barcodeScanner!.show();
  }

  /// Close camera view.
  Future<void> closeVideo() async {
    _barcodeScanner!.hide();
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

    settings = await handleThenable(_barcodeScanner!.getRuntimeSettings());
    obj = json.decode(stringify(settings));
    obj['barcodeFormatIds'] = formats;
    await handleThenable(
        _barcodeScanner!.updateRuntimeSettings(json.encode(obj)));
  }

  /// Convert List<dynamic> to List<Map<dynamic, dynamic>>.
  List<Map<dynamic, dynamic>> _resultWrapper(List<dynamic> barcodeResults) {
    List<Map<dynamic, dynamic>> results = [];

    for (dynamic result in barcodeResults) {
      Map value = json.decode(stringify(result));

      var tmp = Map<dynamic, dynamic>();
      tmp['format'] = value['barcodeFormatString'];
      tmp['text'] = value['barcodeText'];
      tmp['x1'] = value['localizationResult']['x1'];
      tmp['y1'] = value['localizationResult']['y1'];
      tmp['x2'] = value['localizationResult']['x2'];
      tmp['y2'] = value['localizationResult']['y2'];
      tmp['x3'] = value['localizationResult']['x3'];
      tmp['y3'] = value['localizationResult']['y3'];
      tmp['x4'] = value['localizationResult']['x4'];
      tmp['y4'] = value['localizationResult']['y4'];
      tmp['angle'] = value['localizationResult']['angle'];
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

  /// Get all current parameters configured for barcode detection algorithm.
  /// https://www.dynamsoft.com/barcode-reader/parameters/reference/image-parameter/?ver=latest
  Future<String> getParameters() async {
    dynamic settings =
        await handleThenable(_barcodeReader!.outputSettingsToString());
    return stringify(settings);
  }

  /// Set parameters to adjust barcode detection algorithm.
  Future<int> setParameters(String params) async {
    await handleThenable(_barcodeReader!.initRuntimeSettingsWithString(params));
    await handleThenable(
        _barcodeScanner!.initRuntimeSettingsWithString(params));
    return 0;
  }
}
