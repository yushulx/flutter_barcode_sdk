## 5.2.0

* Unified example app: the example now uses `flutter_lite_camera` for the
  camera preview and frame capture on all six platforms, replacing the
  official `camera` plugin, and adds image file scanning (file loading +
  decoding + annotated result view) via `file_selector`.
* Requires `flutter_barcode_sdk_ios` ^1.0.3 and `flutter_barcode_sdk_macos`
  ^1.0.3, which add Swift Package Manager support and restore the missing
  10 pub points (150 -> 160).
* Live scan: track the real camera frame size from every captured frame so
  the preview aspect ratio and the annotation overlay stay aligned even when
  the device delivers frames at a different size than the negotiated one
  (previously the preview could look stretched and the overlay misplaced on
  macOS/desktop).

## 5.1.3

* Requires `flutter_barcode_sdk_ios` ^1.0.2, which fixes the broken generated
  Swift header import that prevented iOS builds after the monorepo
  restructure.

## 5.1.2

* Requires `flutter_barcode_sdk_macos` ^1.0.2, which correctly bundles the
  Dynamsoft `Templates/` and `Models/` resources into macOS apps, fixing
* README: the macOS section now points to the required
  `Copy Dynamsoft Resources` build-phase setup (with an automatic
  `post_install` snippet for `macos/Podfile`).

## 5.1.1

* Requires `flutter_barcode_sdk_macos` ^1.0.1, which fixes broken dylib
  install names that crashed macOS apps at launch.
* Aligned all endorsed platform package constraints to ^1.0.1
  (android / ios / windows / linux / web).
* Example (macOS): fixed the corrupted `project.pbxproj` that excluded
  `GeneratedPluginRegistrant.swift` from compilation; disabled the app
  sandbox so Dynamsoft license activation works; skip `setParameters`
  when `getParameters` returns an empty string.

## 5.1.0

* Added **macOS** platform support via the new `flutter_barcode_sdk_macos`
  package (universal-2 binary, macOS 12+).
* Updated Dynamsoft Barcode Reader SDK:
  - Android / iOS: 11.6.1000
  - Windows / Linux: 11.6.1000 (binaries, headers, and models)
  - Web: 11.6.2000
* All platform example apps are now split from the main example and live
  in each platform package for standalone testing.

## 5.0.0

* **Breaking**: Restructured into a federated plugin.
* The app-facing package (`flutter_barcode_sdk`) now delegates all calls to
  `BarcodeSdkPlatform.instance` from `flutter_barcode_sdk_platform_interface`.
* Native platform implementations are separate packages:
  - `flutter_barcode_sdk_android`
  - `flutter_barcode_sdk_ios`
  - `flutter_barcode_sdk_windows`
  - `flutter_barcode_sdk_linux`
  - `flutter_barcode_sdk_web`
* Each platform package can be published and updated independently,
  each with its own 100MB pub.dev limit.
* Shared data models (`BarcodeResult`, `BarcodeFormat`, `ImagePixelFormat`,
  `ImageRotation`) moved to `flutter_barcode_sdk_platform_interface`.
* Source code is organized as a pub workspace monorepo: all packages live
  under `packages/` and are built, tested, and published independently.
* Each platform package has its own `example/` app for standalone testing.
* The public API (`FlutterBarcodeSdk` class) is unchanged.

## 4.1.0
* Added missing `BarcodeFormat` constants: `NULL`, `ALL`, `DEFAULT`, `TELEPEN`, `TELEPEN_NUMERIC`.
* Added comprehensive documentation for all `BarcodeFormat` constants.
* **Web platform**: Implemented automatic `BigInt` conversion for barcode format values, enabling full support for all 64-bit format constants including postal codes, DotCode, and Pharmacode.
* Organized barcode format constants into logical groups (1D, 2D, GS1, Postal, Pharmacode).
* Improved documentation across all library files.
* Simplified `init()` method by removing redundant platform branches.

## 4.0.1
* Updated the SDK template.

## 4.0.0
* Updated Dynamsoft Barcode SDK for all platforms.

## 3.1.1
* Updated the JavaScript barcode SDK.

## 3.1.0
* Added a `rotation` parameter to `decodeImageBuffer()` method.

## 3.0.5
* Added `errorCode` and `errorMsg` properties to `BarcodeResult`.
* Removed support for the macOS plugin.

## 3.0.4
* Updated SDKs for Android and iOS.

## 3.0.3
* Updated C++ SDK for Windows and Linux.

## 3.0.2
* Fix setBarcodeFormats() for Web.
* Update dependencies for Andriod and iOS.

## 3.0.1
* Fix decodeImageBuffer() for web.

## 3.0.0
* Update iOS SDK to v10.x.

## 2.6.0
* Update Android SDK to v10.x.

## 2.5.1
* Minor updates.

## 2.5.0
* Update SDK for web.

## 2.4.1
* Minor updates.

## 2.4.0
* Update SDK for Windows and Linux.

## 2.3.0
* Update SDK for macOS plugin.

## 2.2.8
* Upgrade Android/iOS native libraries to v9.6.40.

## 2.2.7
* Support AGP 8 for Android.

## 2.2.6
* Fixed Linux plugin build issue.
* Updated dependencies for macOS plugin.

## 2.2.5
* Updated barcode SDK for macOS to v9.6.20.
* Replaced x86_64 with arm64 in macOS plugin.

## 2.2.4
* Updated some barcode SDK versions to v9.6.20.

## 2.2.3
* Fixed `initLicense` return value for Windows and Linux. 

## 2.2.2
* Windows update: Move DecodeImageBuffer method to C++ native thread to avoid UI thread blocking.

## 2.2.1
* Removed useless code in web plugin.
* Optimized scanner example code for web.

## 2.2.0
* Updated Dynamsoft Barcode Reader to v9.6

## 2.1.3
* Updated Android and iOS SDK to v9.4.0.

## 2.1.2
* Fixed JSON format exception caused by special characters in the barcode text.

## 2.1.1
* Fixed uint8 bytes conversion failure in web app.

## 2.1.0
* Added `ImagePixelFormat` enum to support more pixel formats.
* Added `barcodeBytes` property for barcode result.
* Updated Windows, macOS and Linux to Dynamsoft Barcode Reader v9.4.
* Updated web to Dynamsoft Barcode Reader v9.3.1.

## 2.0.3
* Fixed "method not found: 'createDefaultScanner'" in web release mode

## 2.0.2
* Catch license re-activation error when running hot restart for web application.

## 2.0.1
* Web overlay patch for displaying barcode and QR code results on camera preview.

## 2.0.0
* Updated mac SDK to v9.0.0.

## 1.7.0
* Updated web SDK to v9.0.0.
* Bug fixes.

## 1.6.0
* Updated iOS SDK to v9.0.0.
* Bug fixes.

## 1.5.0
* Updated Android SDK to v9.0.0.
* Bug fixes.

## 1.4.1
* Removed unused import: 'dart:html'.

## 1.4.0
* Updated web SDK to v8.8.7.
* Added `setLicense()` method for web.
* Added `decodeImageBuffer()` method for web.

## 1.3.0
* Updated Windows, Linux SDK to v9.0.0.
* Updated Android and iOS SDK to v8.9.3.

## 1.2.2
* Added closeVideo() method for web.

## 1.2.1
* Added file selector to desktop example.

## 1.2.0
* Added two methods: getParameters() and setParameters().
* Optimized code for web plugin.

## 1.1.1
* Fixed the crash issue caused by building release app for Android.

## 1.1.0
* Added setBarcodeFormats() method.

## 1.0.1
* Added macOS platform support.

## 1.0.0
* Added iOS platform support.

## 0.1.2
* Added Linux platform support.

## 0.1.1
* Fixed build issue caused by JS callback.

## 0.1.0
* Added web plugin support.

## 0.0.9
* Added barcode angle value.

## 0.0.8
* Fixed coordinate values in Windows plugin.

## 0.0.7
* Added assets.
* Optimized sample code for Windows desktop.

## 0.0.6
* Added Windows platform support.

## 0.0.5
* Updated README.md.

## 0.0.4
* Added a BarcodeResult class.

## 0.0.3
* Return barcode results as objects instead of strings.

## 0.0.2
* Added decodeImageBuffer() method for scanning barcodes from video stream.

## 0.0.1

* Added decodeFile() method for Android.
