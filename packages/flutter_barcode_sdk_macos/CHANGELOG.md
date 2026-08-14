## 1.0.1

* Fixed broken install names of the bundled `libDynamicPdf.dylib` and
  `libDynamsoftNeuralNetwork.dylib` (now `@rpath/...`), which crashed the
  app at launch with "Library not loaded".
* Example: raised the Podfile platform to macOS 12.0.
* Example: disabled the app sandbox — the Dynamsoft license module needs
  IOKit access and a writable license cache, otherwise license activation
  fails with `EC_LICENSE_BUFFER_FAILED (-20002)`.
* Example: skip `setParameters` when `getParameters` returns an empty
  string (avoids `-10030` / `EC_JSON_PARSE_FAILED`).

## 1.0.0

* Initial release.
* macOS implementation of `flutter_barcode_sdk` using the Dynamsoft
  Barcode Reader C++ SDK (11.6.1000, universal-2 dylibs bundled).
* Implements all methods of `BarcodeSdkPlatform` over the
  `flutter_barcode_sdk` MethodChannel.