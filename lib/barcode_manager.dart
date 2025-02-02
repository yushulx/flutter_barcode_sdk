@JS('Dynamsoft')
library dynamsoft;

import 'dart:async';
import 'dart:convert';
import 'dart:typed_data';
import 'package:js/js.dart';
import 'utils.dart';

@JS()
@anonymous
class CapturedResult {
  external List<CapturedItem> get items;
}

@JS()
@anonymous
class CapturedItem {
  external String get type;
  external String get text;
  external String get formatString;
  external Location get location;
  external int get angle;
  external Uint8List get bytes;
  external int get confidence;
}

@JS()
@anonymous
class Location {
  external List<Point> get points;
}

@JS()
@anonymous
class Point {
  external num get x;
  external num get y;
}

@JS('License.LicenseManager')
class LicenseManager {
  external static PromiseJsImpl<void> initLicense(
      String license, bool executeNow);
}

@JS('Core.CoreModule')
class CoreModule {
  external static PromiseJsImpl<void> loadWasm(List<String> modules);
}

/// Represents the JavaScript Barcode Reader class from Dynamsoft Barcode SDK.
///
/// This class provides methods for barcode decoding and runtime settings manipulation.
/// It is accessed via JavaScript interop (`@JS`) and interacts with the underlying
/// Dynamsoft Barcode Reader (DBR) Web SDK.
///
@JS('CVR.CaptureVisionRouter')
class CaptureVisionRouter {
  /// Creates a new instance of [CaptureVisionRouter].
  ///
  /// This method returns a `PromiseJsImpl` that must be handled asynchronously.
  external static PromiseJsImpl<CaptureVisionRouter> createInstance();

  /// Decodes barcodes from a source.
  ///
  /// The [data] parameter can be a file object or a DSImageData object.
  external PromiseJsImpl<CapturedResult> capture(dynamic data, String template);

  /// Retrieves the simplified runtime settings for barcode decoding.
  external PromiseJsImpl<dynamic> getSimplifiedSettings(String templateName);

  /// Updates simplified runtime settings with a JSON string.
  external PromiseJsImpl<void> updateSettings(
      String templateName, String settings);

  /// Outputs the current runtime settings as a JSON string.
  external PromiseJsImpl<dynamic> outputSettings(String templateName);

  /// Initializes runtime settings from a JSON string.
  external PromiseJsImpl<void> initSettings(String settings);
}

/// Manages barcode decoding operations using the [BarcodeReader] instance.
///
/// This class provides methods for initializing the barcode reader, configuring settings,
/// and decoding barcodes from various sources (file, buffer).
class BarcodeManager {
  CaptureVisionRouter? _barcodeReader;
  String templateName = 'ReadBarcodes_ReadRateFirst';

  /// Creates and initializes a new instance of [CaptureVisionRouter].
  ///
  /// This function is required before performing barcode scans.
  Future<int> initBarcodeSDK() async {
    try {
      _barcodeReader =
          await handleThenable(CaptureVisionRouter.createInstance());
    } catch (e) {
      print(e);
      return -1;
    }

    return 0;
  }

  /// Sets the license key for the barcode reader.
  ///
  /// The [license] parameter should be a valid license string obtained from Dynamsoft.
  /// Returns `0` on success, `-1` on failure.
  Future<int> setLicense(String license) async {
    try {
      await handleThenable(LicenseManager.initLicense(license, true));
      await handleThenable(CoreModule.loadWasm(['dbr']));
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
    try {
      dynamic settings = await handleThenable(
          _barcodeReader!.getSimplifiedSettings(templateName));
      Map obj = json.decode(stringify(settings));
      obj['barcodeSettings']['barcodeFormatIds'] = formats;
      await handleThenable(
          _barcodeReader!.updateSettings(templateName, json.encode(obj)));
    } catch (e) {
      print(e);
      return -1;
    }
    return 0;
  }

  /// Converts raw barcode scan results from a JavaScript array into a Dart-compatible structure.
  ///
  /// This method processes barcode details such as format, position, and raw bytes.
  List<Map<dynamic, dynamic>> _resultWrapper(List<dynamic> barcodeResults) {
    List<Map<dynamic, dynamic>> results = [];

    for (CapturedItem result in barcodeResults) {
      if (result.type != 2) continue;

      var tmp = <dynamic, dynamic>{};
      tmp['format'] = result.formatString;
      tmp['text'] = result.text;
      tmp['x1'] = result.location.points[0].x;
      tmp['y1'] = result.location.points[0].y;
      tmp['x2'] = result.location.points[1].x;
      tmp['y2'] = result.location.points[1].y;
      tmp['x3'] = result.location.points[2].x;
      tmp['y3'] = result.location.points[2].y;
      tmp['x4'] = result.location.points[3].x;
      tmp['y4'] = result.location.points[3].y;
      tmp['angle'] = result.angle;
      tmp['barcodeBytes'] = result.bytes;
      results.add(tmp);
    }

    return results;
  }

  /// Decodes barcodes from an image file.
  ///
  /// The [filename] parameter should be the path or URL of the image file.
  Future<List<Map<dynamic, dynamic>>> decodeFile(String filename) async {
    CapturedResult barcodeResults =
        await handleThenable(_barcodeReader!.capture(filename, templateName));

    return _resultWrapper(barcodeResults.items);
  }

  /// Decodes barcodes from an image buffer.
  ///
  /// The [bytes] parameter contains raw pixel data, while [width], [height], [stride], and [format]
  /// define the dimensions and structure of the image.
  Future<List<Map<dynamic, dynamic>>> decodeImageBuffer(
      Uint8List bytes, int width, int height, int stride, int format) async {
    final dsImage = {
      'bytes': bytes,
      'width': width,
      'height': height,
      'stride': stride,
      'format': format
    };

    CapturedResult barcodeResults =
        await handleThenable(_barcodeReader!.capture(dsImage, templateName));

    return _resultWrapper(barcodeResults.items);
  }

  /// Retrieves the current runtime settings in JSON format.
  ///
  /// Returns a JSON string containing the current barcode recognition parameters.
  Future<String> getParameters() async {
    dynamic settings =
        await handleThenable(_barcodeReader!.outputSettings(templateName));
    return stringify(settings);
  }

  /// Applies runtime settings from a JSON-formatted string.
  ///
  /// This method updates barcode recognition parameters based on the provided [params].
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setParameters(String params) async {
    await handleThenable(_barcodeReader!.initSettings(params));
    return 0;
  }
}
