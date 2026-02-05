@JS('Dynamsoft')
library dynamsoft;

import 'dart:js_interop';
import 'dart:typed_data';
import 'utils.dart';

@JS()
@anonymous
extension type CapturedResult._(JSObject _) implements JSObject {
  external JSArray<CapturedItem> get items;
  external int get errorCode;
  external String get errorString;
}

@JS()
@anonymous
extension type CapturedItem._(JSObject _) implements JSObject {
  external int get type;
  external String get text;
  external String get formatString;
  external Location get location;
  external int get angle;
  external JSAny? get bytes;
  external int get confidence;
}

@JS()
@anonymous
extension type Location._(JSObject _) implements JSObject {
  external JSArray<Point> get points;
}

@JS()
@anonymous
extension type Point._(JSObject _) implements JSObject {
  external JSNumber get x;
  external JSNumber get y;
}

@JS('License.LicenseManager')
extension type LicenseManager._(JSObject _) implements JSObject {
  external static JSPromise<JSAny?> initLicense(String license, bool executeNow);
}

@JS('Core.CoreModule')
extension type CoreModule._(JSObject _) implements JSObject {
  external static JSPromise<JSAny?> loadWasm(JSArray<JSString> modules);
}

/// Represents the JavaScript Barcode Reader class from Dynamsoft Barcode SDK.
///
/// This class provides methods for barcode decoding and runtime settings manipulation.
/// It is accessed via JavaScript interop (`@JS`) and interacts with the underlying
/// Dynamsoft Barcode Reader (DBR) Web SDK.
///
@JS('CVR.CaptureVisionRouter')
extension type CaptureVisionRouter._(JSObject _) implements JSObject {
  /// Creates a new instance of [CaptureVisionRouter].
  ///
  /// This method returns a `JSPromise` that must be handled asynchronously.
  external static JSPromise<CaptureVisionRouter> createInstance();

  /// Decodes barcodes from a source.
  ///
  /// The [data] parameter can be a file object, URL string, or a DSImageData object.
  external JSPromise<CapturedResult> capture(String data, String template);

  /// Decodes barcodes from a DSImageData object.
  @JS('capture')
  external JSPromise<CapturedResult> captureFromImageData(JSObject data, String template);

  /// Retrieves the simplified runtime settings for barcode decoding.
  external JSPromise<JSObject> getSimplifiedSettings(String templateName);

  /// Updates simplified runtime settings with a JSON string.
  external JSPromise<JSAny?> updateSettings(String templateName, JSObject settings);

  /// Outputs the current runtime settings as a JSON string.
  external JSPromise<JSObject> outputSettings(String templateName);

  /// Initializes runtime settings from a JSON string.
  external JSPromise<JSAny?> initSettings(String settings);
}

/// Manages barcode decoding operations using the [BarcodeReader] instance.
///
/// This class provides methods for initializing the barcode reader, configuring settings,
/// and decoding barcodes from various sources (file, buffer).
class BarcodeManager {
  CaptureVisionRouter? _barcodeReader;

  /// Creates and initializes a new instance of [CaptureVisionRouter].
  ///
  /// This function is required before performing barcode scans.
  Future<int> initBarcodeSDK() async {
    try {
      _barcodeReader = await CaptureVisionRouter.createInstance().toDart;
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
      await LicenseManager.initLicense(license, true).toDart;
      await CoreModule.loadWasm(['dbr'.toJS].toJS).toDart;
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
      JSObject rawSettings =
          await _barcodeReader!.getSimplifiedSettings("").toDart;
      Map dartSettings = dartifyObject(rawSettings);
      Map obj = convertBigIntsToInts(dartSettings);
      obj['barcodeSettings']['barcodeFormatIds'] = formats;
      await _barcodeReader!.updateSettings("", jsifyObject(obj)).toDart;
    } catch (e) {
      print(e);
      return -1;
    }
    return 0;
  }

  // Recursively convert all BigInt values to int
  dynamic convertBigIntsToInts(dynamic data) {
    if (data is Map) {
      return data.map((k, v) => MapEntry(k, convertBigIntsToInts(v)));
    } else if (data is List) {
      return data.map(convertBigIntsToInts).toList();
    } else if (data is BigInt) {
      return data.toInt();
    }
    return data;
  }

  /// Converts raw barcode scan results from a JavaScript array into a Dart-compatible structure.
  ///
  /// This method processes barcode details such as format, position, and raw bytes.
  List<Map<dynamic, dynamic>> _resultWrapper(JSArray<CapturedItem> barcodeResults) {
    List<Map<dynamic, dynamic>> results = [];

    List<CapturedItem> items = barcodeResults.toDart;
    for (CapturedItem result in items) {
      if (result.type != 2) continue;

      var tmp = <dynamic, dynamic>{};
      tmp['format'] = result.formatString;
      tmp['text'] = result.text;
      List<Point> points = result.location.points.toDart;
      tmp['x1'] = points[0].x.toDartInt;
      tmp['y1'] = points[0].y.toDartInt;
      tmp['x2'] = points[1].x.toDartInt;
      tmp['y2'] = points[1].y.toDartInt;
      tmp['x3'] = points[2].x.toDartInt;
      tmp['y3'] = points[2].y.toDartInt;
      tmp['x4'] = points[3].x.toDartInt;
      tmp['y4'] = points[3].y.toDartInt;
      tmp['angle'] = result.angle;
      
      final JSAny? bytes = result.bytes;
      if (bytes != null) {
        if (bytes.isA<JSUint8Array>()) {
          tmp['barcodeBytes'] = (bytes as JSUint8Array).toDart;
        } else if (bytes.isA<JSArray>()) {
          final jsArray = bytes as JSArray<JSNumber>;
          final dartList = jsArray.toDart;
          tmp['barcodeBytes'] = Uint8List.fromList(dartList.map((e) => e.toDartInt).toList());
        } else {
          tmp['barcodeBytes'] = Uint8List(0);
        }
      } else {
        tmp['barcodeBytes'] = Uint8List(0);
      }
      
      tmp['errorCode'] = 0;
      tmp['errorMsg'] = '';
      results.add(tmp);
    }

    return results;
  }

  List<Map<dynamic, dynamic>> _errorWrapper(int errorCode, String errorMsg) {
    List<Map<dynamic, dynamic>> results = [];

    var tmp = <dynamic, dynamic>{};
    tmp['format'] = 0;
    tmp['text'] = '';
    tmp['x1'] = 0;
    tmp['y1'] = 0;
    tmp['x2'] = 0;
    tmp['y2'] = 0;
    tmp['x3'] = 0;
    tmp['y3'] = 0;
    tmp['x4'] = 0;
    tmp['y4'] = 0;
    tmp['angle'] = 0;
    tmp['barcodeBytes'] = [];
    tmp['errorCode'] = 0;
    tmp['errorMsg'] = '';
    results.add(tmp);

    return results;
  }

  /// Decodes barcodes from an image file.
  ///
  /// The [filename] parameter should be the path or URL of the image file.
  Future<List<Map<dynamic, dynamic>>> decodeFile(String filename) async {
    CapturedResult barcodeResults =
        await _barcodeReader!.capture(filename, "").toDart;

    if (barcodeResults.errorCode != 0) {
      return _errorWrapper(
          barcodeResults.errorCode, barcodeResults.errorString);
    }
    return _resultWrapper(barcodeResults.items);
  }

  /// Decodes barcodes from an image buffer.
  ///
  /// The [bytes] parameter contains raw pixel data, while [width], [height], [stride], and [format]
  /// define the dimensions and structure of the image.
  Future<List<Map<dynamic, dynamic>>> decodeImageBuffer(Uint8List bytes,
      int width, int height, int stride, int format, int rotation) async {
    final dsImage = jsifyObject({
      'bytes': bytes,
      'width': width,
      'height': height,
      'stride': stride,
      'format': format,
      'orientation': rotation
    });

    CapturedResult barcodeResults =
        await _barcodeReader!.captureFromImageData(dsImage, "").toDart;

    if (barcodeResults.errorCode != 0) {
      return _errorWrapper(
          barcodeResults.errorCode, barcodeResults.errorString);
    }
    return _resultWrapper(barcodeResults.items);
  }

  /// Retrieves the current runtime settings in JSON format.
  ///
  /// Returns a JSON string containing the current barcode recognition parameters.
  Future<String> getParameters() async {
    JSObject settings = await _barcodeReader!.outputSettings("").toDart;
    return stringify(settings);
  }

  /// Applies runtime settings from a JSON-formatted string.
  ///
  /// This method updates barcode recognition parameters based on the provided [params].
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setParameters(String params) async {
    await _barcodeReader!.initSettings(params).toDart;
    return 0;
  }
}
