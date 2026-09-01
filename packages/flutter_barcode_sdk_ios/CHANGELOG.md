## 1.0.3

* Added Swift Package Manager support: the plugin manifest now lives at
  `ios/flutter_barcode_sdk_ios/Package.swift` (Flutter 3.44+ generates the
  local `FlutterFramework` package it references) and sources moved from
  `ios/Classes` to
  `ios/flutter_barcode_sdk_ios/Sources/flutter_barcode_sdk_ios` following
  the official SPM plugin layout. The CocoaPods podspec was updated to the
  new source paths.
* Restores the missing 10 pub points on pub.dev (150 -> 160).

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
