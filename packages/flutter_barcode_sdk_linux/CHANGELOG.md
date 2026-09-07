## 1.0.2

* **Fixed**: `decodeFile()` now uses the same async
  `CFileFetcher`/`StartCapturing` pipeline as `decodeImageBuffer()` instead
  of blocking the platform thread with a synchronous capture.
* **Fixed**: superseded camera-frame calls are answered with an empty
  result instead of leaving Dart futures hanging, and `StartCapturing`
  failures report an error instead of never resolving.
* **Fixed**: memory leaks in the async capture result handling.

## 1.0.1

* Updated Dynamsoft Barcode Reader SDK to 11.6.1000 (binaries, headers and models).

## 1.0.0

* Initial release.
* Native implementation of `flutter_barcode_sdk` using the Dynamsoft
  Barcode Reader SDK via a MethodChannel.
