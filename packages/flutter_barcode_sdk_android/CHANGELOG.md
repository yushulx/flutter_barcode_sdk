## 1.0.2

* **Fixed**: `decodeFile()` runs on a background executor instead of the
  main thread, so decoding no longer blocks the Dart UI.

## 1.0.1

* Updated Dynamsoft Barcode Reader SDK to 11.6.1000.

## 1.0.0

* Initial release.
* Native implementation of `flutter_barcode_sdk` using the Dynamsoft
  Barcode Reader SDK via a MethodChannel.
