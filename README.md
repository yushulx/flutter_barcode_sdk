# flutter_barcode_sdk
![pub.dev](https://img.shields.io/pub/v/flutter_barcode_sdk.svg)

The **Flutter Barcode SDK** is a wrapper for the [Dynamsoft Barcode Reader SDK](https://www.dynamsoft.com/barcode-reader/overview/). It supports multiple platforms, including **Android**, **iOS**, **Web**, **Windows**, **Linux** and **macOS**, and can read various barcode types such as linear barcode, QR Code, DataMatrix, MaxiCode, PDF417, and more. This SDK encapsulates the low-level decoding functions of the Dynamsoft Barcode Reader, enabling both file and image buffer decoding. The project is actively maintained by community contributors.

> **Note:** For live camera scenarios, it is recommended to use the official [Dynamsoft Capture Vision Flutter Edition](https://pub.dev/packages/dynamsoft_capture_vision_flutter), as it offers better performance than combining the [Flutter camera plugin](https://pub.dev/packages/camera) with the Flutter Barcode SDK.

## **Table of Contents**
1. [Getting a License Key](#getting-a-license-key)
2. [Supported Platforms](#supported-platforms)
3. [Supported Barcode Symbologies](#supported-barcode-symbologies)
4. [Build Configuration](#build-configuration)
5. [API Compatibility](#api-compatibility)
6. [Usage](#usage)
7. [Examples](#try-barcode-decoding-example)

## **Getting a License Key**
[![](https://img.shields.io/badge/Get-30--day%20FREE%20Trial-blue)](https://www.dynamsoft.com/customer/license/trialLicense/?product=dcv&package=cross-platform)

## **Supported Platforms**
- **Android**
- **iOS**
- **Windows**
- **Linux**
- **macOS**
- **Web**

## **Supported Barcode Symbologies**
### **Linear Barcodes (1D)**
- Code 39 (including Code 39 Extended)
- Code 93
- Code 128
- Codabar
- Interleaved 2 of 5
- EAN-8
- EAN-13
- UPC-A
- UPC-E
- Industrial 2 of 5

### **2D Barcodes**
- QR Code (including Micro QR Code and Model 1)
- Data Matrix
- PDF417 (including Micro PDF417)
- Aztec Code
- MaxiCode (mode 2-5)
- DotCode

### **Others**
- Patch Code
- GS1 Composite Code
- GS1 DataBar
  - Omnidirectional,
  - Truncated, Stacked, Stacked
  - Omnidirectional, Limited,
  - Expanded, Expanded Stacked

- Postal Codes
  - USPS Intelligent Mail
  - Postnet
  - Planet
  - Australian Post
  - UK Royal Mail

## **Build Configuration**

### Android
Set the minimum SDK version in `android/app/build.gradle`.

```
minSdkVersion 21
```

### iOS
Add camera usage descriptions to `ios/Runner/Info.plist`:

```
<key>NSCameraUsageDescription</key>
<string>Can I use the camera please?</string>
<key>NSMicrophoneUsageDescription</key>
<string>Can I use the mic please?</string>
```

### Desktop

**Windows & Linux**

Install `CMake` and `platform-specific C++ compiler`.

**macOS**

Install `Xcode`.

To make the demo app work on macOS:
- Disable `com.apple.security.app-sandbox` and enable `com.apple.security.files.user-selected.read-write` in `example/macos/Runner/DebugProfile.entitlements`:
    
  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
  <plist version="1.0">
  <dict>
    <key>com.apple.security.app-sandbox</key>
    <false/>
    <key>com.apple.security.cs.allow-jit</key>
    <true/>
    <key>com.apple.security.network.server</key>
    <true/>
    <key>com.apple.security.files.user-selected.read-write</key>
    <true/>
  </dict>
  </plist>
  ```    

- If `DCV` package is not found, run the command:

  ```bash
  pod repo add master https://github.com/CocoaPods/Specs
  ```
  The command manually adds the CocoaPods `master` spec repository to the local machine to resolve the issue.

### Web
In `index.html`, include:

```html
<script src="https://cdn.jsdelivr.net/npm/dynamsoft-javascript-barcode@9.6.42/dist/dbr.js"></script>
```

## **API Compatibility**
| Methods      | Android |    iOS | Windows | Linux | macOS | Web|
| ----------- | ----------- | ----------- | ----------- |----------- |----------- |----------- |
| `Future<void> setLicense(String license) async`     | :heavy_check_mark:       | :heavy_check_mark:   | :heavy_check_mark:      | :heavy_check_mark:      |:heavy_check_mark:      | :heavy_check_mark:    |
| `Future<List<BarcodeResult>> decodeFile(String filename) async`     | :heavy_check_mark:      | :heavy_check_mark:   | :heavy_check_mark:      |:heavy_check_mark:      | :heavy_check_mark:     |:heavy_check_mark:      |
| `Future<List<BarcodeResult>> decodeImageBuffer(Uint8List bytes, int width, int height, int stride, int format) async`     | :heavy_check_mark:      | :heavy_check_mark:   | :heavy_check_mark:      |:heavy_check_mark:      | :heavy_check_mark:     |:heavy_check_mark:    |
| `Future<int> setBarcodeFormats(int formats) async`     | :heavy_check_mark:       | :heavy_check_mark:   | :heavy_check_mark:       | :heavy_check_mark:       |:heavy_check_mark:      | :heavy_check_mark:     |
| `Future<String> getParameters() async`     | :heavy_check_mark:         | :heavy_check_mark:   | :heavy_check_mark:       | :heavy_check_mark:        |:heavy_check_mark:       | :heavy_check_mark:     |
| `Future<int> setParameters(String params)` async | :heavy_check_mark:         |:heavy_check_mark:   | :heavy_check_mark:       | :heavy_check_mark:        |:heavy_check_mark:      | :heavy_check_mark:     |
| `Future<void> init()` async | :heavy_check_mark:         |:heavy_check_mark:   | :heavy_check_mark:       | :heavy_check_mark:        |:heavy_check_mark:      | :heavy_check_mark:     |

## **Usage**
- Initialize Flutter barcode SDK and set the license key:
    
  ```dart
  _barcodeReader = FlutterBarcodeSdk();
  await _barcodeReader.setLicense('LICENSE-KEY');
  await _barcodeReader.init();
  ```

- Read barcodes from an image file:

  ```dart
  List<BarcodeResult> results = await _barcodeReader.decodeFile(image-path);
  ```

- Read barcodes from an image buffer:


  ```dart
  import 'dart:ui' as ui;
  Uint8List fileBytes = await file.readAsBytes();
  ui.Image image = await decodeImageFromList(fileBytes);

  ByteData byteData = await image.toByteData(
      format: ui.ImageByteFormat.rawRgba);
  List<BarcodeResult> results =
      await _barcodeReader.decodeImageBuffer(
          byteData.buffer.asUint8List(),
          image.width,
          image.height,
          byteData.lengthInBytes ~/ image.height,
          ImagePixelFormat.IPF_ARGB_8888.index);
  ```

- Set barcode formats:

  ```dart
  int ret = await _barcodeReader.setBarcodeFormats(BarcodeFormat.CODE_39 | BarcodeFormat.CODABAR | BarcodeFormat.QR_CODE | BarcodeFormat.DATAMATRIX);
  ```

- Get current barcode detection parameters:
    
  ```dart
  String params = await _barcodeReader.getParameters();
  ```

- Set barcode detection parameters:
    
  ```dart
  int ret = await _barcodeReader.setParameters(params);
  ```


## **Try Barcode Decoding Example**

### Android/iOS
The example demonstrates how to use the Flutter Barcode SDK to read barcodes from an image file and decode the barcode image buffer from the camera stream on Android and iOS.

```
cd example
flutter run -d <device>
```

- **Camera Stream**

    ![flutter barcode scanner](https://www.dynamsoft.com/codepool/img/2021/flutter-barcode-scanner-camera.gif)

- **Image File**

    ![flutter barcode reader](https://www.dynamsoft.com/codepool/img/2021/flutter-picture-barcode-scan.jpg)


### Windows, Linux and macOS
Run the desktop barcode reader and scanner application on Windows, Linux or macOS:

```bash
cd example
# Windows
flutter run -d windows

# Linux
flutter run -d linux

# macOS
flutter run -d macos
```

- **Image File**

    ![flutter desktop barcode reader](https://www.dynamsoft.com/codepool/img/2025/01/flutter-barcode-reader-windows-linux-macos.png)

- **Camera Stream**

    ![flutter desktop barcode scanner](https://www.dynamsoft.com/codepool/img/2025/01/flutter-barcode-scanner-windows-linux-macos.png)


### Web Browser

```bash
cd example
flutter run -d chrome
```

![flutter web barcode reader](https://www.dynamsoft.com/codepool/img/2021/flutter-web-barcode-sdk.png)








