## 1.0.2

* Fixed the generated Swift header import in `FlutterBarcodeSdkPlugin.m`
  (`flutter_barcode_sdk-Swift.h` -> `flutter_barcode_sdk_ios-Swift.h`),
  which broke iOS builds after the monorepo restructure.
* Example (iOS): fixed the app being killed on "Start Video Scan" by adding
  `NSMicrophoneUsageDescription` and setting `enableAudio: false`.

## 1.0.1

* Updated Dynamsoft Barcode Reader SDK to 11.6.1000.

## 1.0.0

* Initial release.
* Native implementation of `flutter_barcode_sdk` using the Dynamsoft
  Barcode Reader SDK via a MethodChannel.
