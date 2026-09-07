## 1.1.0

* Self-host the Dynamsoft Barcode Reader JS/WASM bundle as package assets:
  the plugin loads it automatically — no CDN `<script>` tag is needed in
  `index.html`. Apps that already load the JS SDK themselves keep working
  unchanged.
* `decodeImageBuffer()` now passes a copy of the pixel buffer to the
  engine, and decode errors surface through `errorCode`/`errorMsg`.

## 1.0.1

* Updated the JavaScript barcode SDK to 11.6.2000.

## 1.0.0

* Initial release.
* Web implementation of `flutter_barcode_sdk` using the Dynamsoft
  Barcode Reader JS SDK via `dart:js_interop`.
* Implements all methods of `BarcodeSdkPlatform` without a MethodChannel,
  performing barcode decoding directly in Dart through JS interop.
