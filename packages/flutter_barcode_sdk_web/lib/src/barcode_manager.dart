import 'dart:async';
import 'dart:js_interop';
import 'dart:typed_data';

import 'package:web/web.dart' as web;

import 'utils.dart';

/// Browser implementation backed by the self-hosted Dynamsoft Barcode Reader
/// bundle.
///
/// All namespace navigation goes through `Reflect` reads instead of typed
/// interop access: the bundle builds its namespaces as null-prototype
/// objects, and a library-level `@JS('Dynamsoft')` annotation would also
/// prefix these helper bindings before the SDK is loaded.
@JS('Reflect.get')
external JSAny? _getPropertyValue(JSAny? object, JSAny key);

@JS('Reflect.set')
external JSBoolean _setPropertyValue(JSAny? object, JSAny key, JSAny? value);

@JS('Reflect.apply')
external JSAny? _applyFunction(
  JSAny? function,
  JSAny? thisArgument,
  JSArray<JSAny?> arguments,
);

@JS('Object.keys')
external JSArray<JSString> _objectKeysOf(JSAny? object);

/// Coerces a JS value (e.g. a `BigInt`) to a string, mirroring `String(v)`.
@JS('String')
external String _jsStringOf(JSAny? value);

/// Manages barcode decoding operations using the [BarcodeManager] instance.
///
/// This class provides methods for initializing the barcode reader, configuring settings,
/// and decoding barcodes from various sources (file, buffer).
class BarcodeManager {
  /// Root directory of the self-hosted Dynamsoft Barcode Reader bundle as
  /// served by Flutter's asset system. The engine derives its versioned
  /// resource directories (`dynamsoft-barcode-reader-bundle@<version>/dist/`)
  /// from this root, mirroring the packaged npm layout.
  static const _assetRoot =
      'assets/packages/flutter_barcode_sdk_web/assets/dbr/';
  static const _bundleUrl =
      '${_assetRoot}dynamsoft-barcode-reader-bundle@11.6.2000/dist/dbr.bundle.js';

  /// The `CaptureVisionRouter` instance, kept untyped: the bundle's
  /// namespaces are null-prototype objects that Dart interop type checks
  /// reject.
  JSAny? _barcodeReader;
  Future<void>? _loadFuture;

  /// Loads the self-hosted JS/WASM bundle once before any interop call.
  ///
  /// Apps that already load the SDK themselves (e.g. a CDN `<script>` tag in
  /// `index.html`) keep working untouched: the injection is skipped whenever
  /// `window.Dynamsoft` is already defined.
  Future<void> _ensureSdkLoaded() => _loadFuture ??= _loadSdkOnce();

  Future<void> _loadSdkOnce() async {
    if (!_hasDynamsoft()) {
      final completion = Completer<void>();
      final script = web.HTMLScriptElement()
        ..async = true
        ..src = _bundleUrl;
      script.addEventListener(
        'load',
        ((web.Event _) => completion.complete()).toJS,
      );
      script.addEventListener(
        'error',
        ((web.Event _) => completion.completeError(
              StateError(
                  'Unable to load the self-hosted Dynamsoft Barcode Reader bundle.'),
            )).toJS,
      );
      web.document.head!.append(script);
      await completion.future;
    }
    // The bundle normally attaches its namespaces synchronously on the script
    // `load` event, but under debug module loaders the UMD factory can settle
    // a moment later, so poll briefly instead of failing on the first read.
    final deadline = DateTime.now().add(const Duration(seconds: 15));
    while (true) {
      final paths = _engineResourcePaths();
      if (paths != null) {
        // Must be set before the WASM engine loads: the worker and the model,
        // template, and parser resources are fetched relative to this root.
        _set(paths, 'rootDirectory', _assetRoot.toJS);
        return;
      }
      if (DateTime.now().isAfter(deadline)) {
        throw StateError(
            'The Dynamsoft Barcode Reader bundle loaded but never exposed its Core namespace.');
      }
      await Future<void>.delayed(const Duration(milliseconds: 50));
    }
  }

  /// The `Core.CoreModule.engineResourcePaths` object, or null until the
  /// bundle has fully attached its namespaces to `window.Dynamsoft`.
  JSAny? _engineResourcePaths() {
    final dynamsoft = _get(globalContext, 'Dynamsoft');
    if (dynamsoft == null) return null;
    final core = _get(dynamsoft, 'Core');
    if (core == null) return null;
    final coreModule = _get(core, 'CoreModule');
    if (coreModule == null) return null;
    return _get(coreModule, 'engineResourcePaths');
  }

  bool _hasDynamsoft() => _get(globalContext, 'Dynamsoft') != null;

  JSAny? _requireDynamsoft() {
    final dynamsoft = _get(globalContext, 'Dynamsoft');
    if (dynamsoft == null) {
      throw StateError(
          'The Dynamsoft Barcode Reader browser bundle is not available.');
    }
    return dynamsoft;
  }

  JSAny? _requireRouter() {
    final router = _barcodeReader;
    if (router == null) {
      throw StateError('Call init() before decoding barcodes on the web.');
    }
    return router;
  }

  /// Reads a property through Reflect.get, which works on every object shape.
  static JSAny? _get(JSAny? object, String key) =>
      _getPropertyValue(object, key.toJS);

  /// Writes a property through Reflect.set, which works on every object shape.
  static void _set(JSAny? object, String key, JSAny? value) {
    _setPropertyValue(object, key.toJS, value);
  }

  /// Invokes a JS function with [thisArgument] bound and awaits the result
  /// when it is a promise.
  static Future<JSAny?> _call(
    JSAny? target,
    String method,
    List<JSAny?> arguments,
  ) async {
    final function = _get(target, method);
    if (function == null) {
      throw StateError(
          'The Dynamsoft Barcode Reader bundle does not expose `$method`.');
    }
    final value = _applyFunction(function, target, arguments.toJS);
    if (value != null && value.isA<JSPromise<JSAny?>>()) {
      return (value as JSPromise<JSAny?>).toDart;
    }
    return value;
  }

  static Object? _dartify(JSAny? value) {
    if (value == null || value.isUndefinedOrNull) return null;
    if (value.isA<JSBoolean>()) return (value as JSBoolean).toDart;
    if (value.typeofEquals('bigint')) {
      return BigInt.parse(_jsStringOf(value));
    }
    if (value.isA<JSNumber>()) {
      final number = (value as JSNumber).toDartDouble;
      return number == number.truncateToDouble() ? number.toInt() : number;
    }
    if (value.isA<JSString>()) return (value as JSString).toDart;
    if (value.isA<JSUint8Array>()) return (value as JSUint8Array).toDart;
    if (value.isA<JSArray>()) {
      final array = value as JSArray<JSAny?>;
      return List<Object?>.generate(
        array.length,
        (index) => _dartify(array[index]),
      );
    }
    if (value.typeofEquals('object')) {
      // Plain objects, including the null-prototype namespaces the bundle
      // builds with Object.create(null).
      final map = <String, Object?>{};
      final keys = _objectKeysOf(value);
      for (var index = 0; index < keys.length; index++) {
        final key = keys[index].toDart;
        map[key] = _dartify(_get(value, key));
      }
      return map;
    }
    return null;
  }

  /// Creates and initializes a new instance of the `CaptureVisionRouter`.
  ///
  /// This function is required before performing barcode scans.
  Future<int> initBarcodeSDK() async {
    try {
      await _ensureSdkLoaded();
      final dynamsoft = _requireDynamsoft();
      final routerType =
          _get(_get(dynamsoft, 'CVR'), 'CaptureVisionRouter');
      final router = await _call(routerType, 'createInstance', const []);
      if (router == null) {
        throw StateError(
            'The Dynamsoft Barcode Reader bundle did not return a CaptureVisionRouter.');
      }
      _barcodeReader = router;
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
      await _ensureSdkLoaded();
      final dynamsoft = _requireDynamsoft();
      // `initLicense` is a static member on the LicenseManager type object.
      final licenseManager = _get(_get(dynamsoft, 'License'), 'LicenseManager');
      await _call(licenseManager, 'initLicense', [license.toJS, true.toJS]);
      final coreModule = _get(_get(dynamsoft, 'Core'), 'CoreModule');
      await _call(coreModule, 'loadWasm', [
        ['dbr'.toJS].toJS
      ]);
    } catch (e) {
      print(e);
      return -1;
    }
    return 0;
  }

  /// Sets the barcode formats to be recognized.
  ///
  /// The [formats] parameter specifies the barcode types to detect.
  /// Format values are automatically converted to JavaScript `BigInt` to
  /// support the full 64-bit range, including formats like postal codes,
  /// DotCode, and Pharmacode.
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setBarcodeFormats(int formats) async {
    try {
      final router = _requireRouter();
      JSObject rawSettings =
          await _call(router, 'getSimplifiedSettings', [''.toJS])
              as JSObject;
      Map dartSettings = dartifyObject(rawSettings);
      Map obj = convertBigIntsToInts(dartSettings);
      obj['barcodeSettings']['barcodeFormatIds'] =
          BigInt.from(formats).toUnsigned(64);
      await _call(router, 'updateSettings', [''.toJS, jsifyObject(obj)]);
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

  /// Wraps a raw `capture()` result into the legacy map shape consumed by
  /// `convertResults`; only barcode items (`type == 2`) are kept.
  List<Map<dynamic, dynamic>> _resultWrapper(JSAny? rawResult) {
    List<Map<dynamic, dynamic>> results = [];

    final result = _dartify(rawResult);
    if (result is! Map) return _errorWrapper(0, '');

    final Object? errorCode = result['errorCode'];
    if (errorCode is num && errorCode != 0) {
      return _errorWrapper(
          errorCode.toInt(), result['errorString'] as String? ?? '');
    }

    for (final item in (result['items'] as List? ?? const [])) {
      if (item is! Map || item['type'] != 2) continue;

      final location = item['location'] as Map? ?? const {};
      final points = location['points'] as List? ?? const [];

      final tmp = <dynamic, dynamic>{};
      tmp['format'] = item['formatString'] as String? ?? '';
      tmp['text'] = item['text'] as String? ?? '';
      tmp['x1'] = _coordOf(points, 0, 'x');
      tmp['y1'] = _coordOf(points, 0, 'y');
      tmp['x2'] = _coordOf(points, 1, 'x');
      tmp['y2'] = _coordOf(points, 1, 'y');
      tmp['x3'] = _coordOf(points, 2, 'x');
      tmp['y3'] = _coordOf(points, 2, 'y');
      tmp['x4'] = _coordOf(points, 3, 'x');
      tmp['y4'] = _coordOf(points, 3, 'y');
      tmp['angle'] = item['angle'] is num ? (item['angle'] as num).toInt() : 0;

      final bytes = item['bytes'];
      if (bytes is Uint8List) {
        tmp['barcodeBytes'] = bytes;
      } else if (bytes is List) {
        tmp['barcodeBytes'] = Uint8List.fromList(
            bytes.whereType<num>().map((e) => e.toInt()).toList());
      } else {
        tmp['barcodeBytes'] = Uint8List(0);
      }

      tmp['errorCode'] = 0;
      tmp['errorMsg'] = '';
      results.add(tmp);
    }

    return results;
  }

  static int _coordOf(List points, int index, String axis) {
    if (index >= points.length) return 0;
    final point = points[index];
    if (point is! Map) return 0;
    final value = point[axis];
    return value is num ? value.toInt() : 0;
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
    tmp['errorCode'] = errorCode;
    tmp['errorMsg'] = errorMsg;
    results.add(tmp);

    return results;
  }

  /// Decodes barcodes from an image file.
  ///
  /// The [filename] parameter should be the path or URL of the image file.
  Future<List<Map<dynamic, dynamic>>> decodeFile(String filename) async {
    final rawResult =
        await _call(_requireRouter(), 'capture', [filename.toJS, ''.toJS]);
    return _resultWrapper(rawResult);
  }

  /// Decodes barcodes from an image buffer.
  ///
  /// The [bytes] parameter contains raw pixel data, while [width], [height], [stride], and [format]
  /// define the dimensions and structure of the image.
  Future<List<Map<dynamic, dynamic>>> decodeImageBuffer(Uint8List bytes,
      int width, int height, int stride, int format, int rotation) async {
    final imageData = JSObject();
    // Hand the SDK a copy of the bytes: the recognition pipeline may
    // transfer the underlying ArrayBuffer into its worker, which detaches
    // the caller's buffer and breaks any later read of it.
    _set(imageData, 'bytes', Uint8List.fromList(bytes).toJS);
    _set(imageData, 'width', width.toJS);
    _set(imageData, 'height', height.toJS);
    _set(imageData, 'stride', stride.toJS);
    _set(imageData, 'format', format.toJS);
    _set(imageData, 'orientation', rotation.toJS);

    final rawResult =
        await _call(_requireRouter(), 'capture', [imageData, ''.toJS]);
    return _resultWrapper(rawResult);
  }

  /// Retrieves the current runtime settings in JSON format.
  ///
  /// Returns a JSON string containing the current barcode recognition parameters.
  Future<String> getParameters() async {
    final rawSettings =
        await _call(_requireRouter(), 'outputSettings', [''.toJS]);
    return stringify(rawSettings! as JSObject);
  }

  /// Applies runtime settings from a JSON-formatted string.
  ///
  /// This method updates barcode recognition parameters based on the provided [params].
  ///
  /// Returns `0` on success, or an error code on failure.
  Future<int> setParameters(String params) async {
    try {
      await _call(_requireRouter(), 'initSettings', [params.toJS]);
    } catch (e) {
      print(e);
      return -1;
    }
    return 0;
  }
}
