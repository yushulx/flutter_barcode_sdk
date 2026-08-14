## 1.0.1

* Updated the JavaScript barcode SDK to 11.6.2000.

## 1.0.0

* Initial release.
* Web implementation of `flutter_barcode_sdk` using the Dynamsoft
  Barcode Reader JS SDK via `dart:js_interop`.
* Implements all methods of `BarcodeSdkPlatform` without a MethodChannel,
  performing barcode decoding directly in Dart through JS interop.
