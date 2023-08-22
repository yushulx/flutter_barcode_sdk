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
