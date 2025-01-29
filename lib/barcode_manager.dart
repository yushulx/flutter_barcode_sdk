@JS('Dynamsoft')
library dynamsoft;

import 'dart:async';
import 'dart:convert';
import 'dart:typed_data';
import 'package:flutter_barcode_sdk/dynamsoft_barcode.dart';
import 'package:js/js.dart';
import 'utils.dart';

/// Represents the JavaScript Barcode Reader class from Dynamsoft Barcode SDK.
///
/// This class provides methods for barcode decoding and runtime settings manipulation.
/// It is accessed via JavaScript interop (`@JS`) and interacts with the underlying
/// Dynamsoft Barcode Reader (DBR) Web SDK.
///
@JS('DBR.BarcodeReader')
class BarcodeReader {
  /// Creates a new instance of [BarcodeReader].
  ///
  /// This method returns a `PromiseJsImpl` that must be handled asynchronously.
  external static PromiseJsImpl<BarcodeReader> createInstance();

  /// Decodes barcodes from an image file.
  ///
  /// The [file] parameter can be an image URL or a `File` object.
  external PromiseJsImpl<List<dynamic>> decode(dynamic file);

  /// Decodes barcodes from an image buffer.
  ///
  /// The [bytes] parameter contains the raw pixel data, and [width], [height], and [stride]
  /// define the dimensions and memory layout of the image. The [format] specifies the pixel format.
  external PromiseJsImpl<List<dynamic>> decodeBuffer(
      Uint8List bytes, int width, int height, int stride, int format);

  /// Retrieves the current runtime settings for barcode decoding.
  external PromiseJsImpl<dynamic> getRuntimeSettings();

  /// Updates runtime settings with a JSON string.
  ///
  /// The [settings] string should be formatted according to Dynamsoft's runtime settings structure.
  external PromiseJsImpl<void> updateRuntimeSettings(String settings);

  /// Outputs the current runtime settings as a JSON string.
  external PromiseJsImpl<dynamic> outputRuntimeSettingsToString();

  /// Initializes runtime settings from a JSON string.
  external PromiseJsImpl<void> initRuntimeSettingsWithString(String settings);

  /// Sets the license key for the barcode reader.
  external static set license(String license);
}

/// Manages barcode decoding operations using the [BarcodeReader] instance.
///
/// This class provides methods for initializing the barcode reader, configuring settings,
/// and decoding barcodes from various sources (file, buffer).
class BarcodeManager {
  BarcodeReader? _barcodeReader;

  /// Converts raw barcode results into a list of [BarcodeResult] objects.
  ///
  /// This function maps the dynamically structured JSON results into a Dart-friendly format.
  List<BarcodeResult> callbackResults(List<Map<dynamic, dynamic>> results) {
    return convertResults(results);
  }

  /// Creates and initializes a new instance of [BarcodeReader].
  ///
  /// This function is required before performing barcode scans.
  Future<int> initBarcodeSDK() async {
    _barcodeReader = await handleThenable(BarcodeReader.createInstance());
    return 0;
  }

  /// Sets the license key for the barcode reader.
  ///
  /// The [license] parameter should be a valid license string obtained from Dynamsoft.
  /// Returns `0` on success, `-1` on failure.
  Future<int> setLicense(String license) async {
    try {
      BarcodeReader.license = license;
    } catch (e) {
      print(e);
      return -1;
    }
    return 0;
  }

  /// Sets the barcode formats to be recognized.
  ///
  /// The [formats] parameter specifies the barcode types to detect.
  /// Returns `0` on success, or an error code on failure.
  Future<int> setBarcodeFormats(int formats) async {
    updateSettings(formats);
    return 0;
  }

  /// Updates runtime settings for barcode decoding.
  ///
  /// This method modifies the barcode format settings and applies them to the [BarcodeReader].
  Future<void> updateSettings(int formats) async {
    dynamic settings =
        await handleThenable(_barcodeReader!.getRuntimeSettings());
    Map obj = json.decode(stringify(settings));
    obj['barcodeFormatIds'] = formats;
    await handleThenable(
        _barcodeReader!.updateRuntimeSettings(json.encode(obj)));
  }

  /// Converts raw barcode scan results from a JavaScript array into a Dart-compatible structure.
  ///
  /// This method processes barcode details such as format, position, and raw bytes.
  List<Map<dynamic, dynamic>> _resultWrapper(List<dynamic> barcodeResults) {
    List<Map<dynamic, dynamic>> results = [];

    for (dynamic result in barcodeResults) {
      Map value = json.decode(stringify(result));

      var tmp = <dynamic, dynamic>{};
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
      tmp['barcodeBytes'] = value['barcodeBytes'];
      results.add(tmp);
    }

    return results;
  }

  /// Decodes barcodes from an image file.
  ///
  /// The [filename] parameter should be the path or URL of the image file.
  Future<List<Map<dynamic, dynamic>>> decodeFile(String filename) async {
    List<dynamic> barcodeResults =
        await handleThenable(_barcodeReader!.decode(filename));

    return _resultWrapper(barcodeResults);
  }

  /// Decodes barcodes from an image buffer.
  ///
  /// The [bytes] parameter contains raw pixel data, while [width], [height], [stride], and [format]
  /// define the dimensions and structure of the image.
  Future<List<Map<dynamic, dynamic>>> decodeImageBuffer(
      Uint8List bytes, int width, int height, int stride, int format) async {
    List<dynamic> barcodeResults = await handleThenable(
        _barcodeReader!.decodeBuffer(bytes, width, height, stride, format));

    return _resultWrapper(barcodeResults);
  }

  /// Retrieves the current runtime settings in JSON format.
  ///
  /// Returns a JSON string containing the current barcode recognition parameters.
  Future<String> getParameters() async {
    dynamic settings =
        await handleThenable(_barcodeReader!.outputRuntimeSettingsToString());
    return stringify(settings);
  }

  /// Applies runtime settings from a JSON-formatted string.
  ///
  /// This method updates barcode recognition parameters based on the provided [params].
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setParameters(String params) async {
    await handleThenable(_barcodeReader!.initRuntimeSettingsWithString(params));
    return 0;
  }
}
