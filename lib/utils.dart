import 'dart:js_interop';
import 'dart:js_interop_unsafe';
import 'dart:typed_data';

@JS('JSON.stringify')
external String stringify(JSObject obj);

@JS('console.log')
external void log(JSAny obj);

@JS('alert')
external void alert(JSAny obj);

/// Converts a Dart object (Map, List, or primitive) to a JSObject.
JSObject jsifyObject(Object dartObject) {
  return _jsifyValue(dartObject) as JSObject;
}

JSAny? _jsifyValue(dynamic value) {
  if (value == null) {
    return null;
  } else if (value is bool) {
    return value.toJS;
  } else if (value is int) {
    return value.toJS;
  } else if (value is double) {
    return value.toJS;
  } else if (value is String) {
    return value.toJS;
  } else if (value is Uint8List) {
    // Convert Uint8List to JSUint8Array for proper JS interop
    return value.toJS;
  } else if (value is List) {
    return value.map(_jsifyValue).toList().toJS;
  } else if (value is Map) {
    final jsObject = JSObject();
    value.forEach((k, v) {
      jsObject[k.toString()] = _jsifyValue(v);
    });
    return jsObject;
  } else {
    return value.toString().toJS;
  }
}

/// Converts a JSObject to a Dart Map.
Map dartifyObject(JSObject jsObject) {
  return _dartifyValue(jsObject) as Map;
}

dynamic _dartifyValue(JSAny? value) {
  if (value == null || value.isUndefinedOrNull) {
    return null;
  }
  
  if (value.typeofEquals('boolean')) {
    return (value as JSBoolean).toDart;
  }
  
  if (value.typeofEquals('number')) {
    final jsNum = value as JSNumber;
    // Check if it's an integer
    final doubleVal = jsNum.toDartDouble;
    if (doubleVal == doubleVal.truncate()) {
      return doubleVal.truncate();
    }
    return doubleVal;
  }
  
  if (value.typeofEquals('string')) {
    return (value as JSString).toDart;
  }
  
  if (value.typeofEquals('bigint')) {
    return BigInt.parse(_bigIntToString(value));
  }
  
  if (value.isA<JSArray>()) {
    final jsArray = value as JSArray;
    final List result = [];
    for (int i = 0; i < jsArray.length; i++) {
      result.add(_dartifyValue(jsArray[i]));
    }
    return result;
  }
  
  if (value.typeofEquals('object')) {
    final jsObj = value as JSObject;
    final Map<String, dynamic> result = {};
    final keys = _getObjectKeys(jsObj);
    for (final key in keys) {
      result[key] = _dartifyValue(jsObj[key]);
    }
    return result;
  }
  
  return value.toString();
}

@JS('Object.keys')
external JSArray<JSString> _objectKeys(JSObject obj);

List<String> _getObjectKeys(JSObject obj) {
  final jsKeys = _objectKeys(obj);
  final List<String> result = [];
  for (int i = 0; i < jsKeys.length; i++) {
    result.add(jsKeys[i].toDart);
  }
  return result;
}

@JS('String')
external String _bigIntToString(JSAny bigInt);
