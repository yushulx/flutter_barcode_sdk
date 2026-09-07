## 1.0.2

* **Fixed**: `decodeFile()` runs on the background decode worker thread
  instead of the platform thread, so decoding no longer blocks the Dart UI.
* **Fixed**: cleared camera-frame tasks now answer their callers with an
  empty result instead of leaving Dart futures hanging, and pending
  method-call replies are mutex-protected against worker/platform thread
  races.
* **Fixed**: unreleased `CCapturedResult` objects in the decode result
  handling (error and empty-result paths).

## 1.0.1

* Updated Dynamsoft Barcode Reader SDK to 11.6.1000 (binaries, headers and models).

## 1.0.0

* Initial release.
* Native implementation of `flutter_barcode_sdk` using the Dynamsoft
  Barcode Reader SDK via a MethodChannel.
