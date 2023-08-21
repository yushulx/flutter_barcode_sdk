# flutter_barcode_sdk
![pub.dev](https://img.shields.io/pub/v/flutter_barcode_sdk.svg)

The Flutter barcode Qr reading SDK is a wrapper for [Dynamsoft Barcode Reader SDK](https://www.dynamsoft.com/barcode-reader/overview/). It aims to cover Android, iOS, Web, Windows, Linux and macOS, supporting linear barcode, QR Code, DataMatrix, MaxiCode, PDF417, etc. The SDK encapsulates the low-level decoding functions of the Dynamsoft Barcode Reader SDK, including file decoding and image buffer decoding. The project is maintained by community contributors.

For live camera scenarios, it is recommended to use the official [Dynamsoft Capture Vision Flutter Edition](https://pub.dev/packages/dynamsoft_capture_vision_flutter), which is better than combining the [Flutter camera plugin](https://pub.dev/packages/camera) and the Flutter barcode SDK.

## Getting a License Key for Dynamsoft Barcode Reader
[![](https://img.shields.io/badge/Get-30--day%20FREE%20Trial-blue)](https://www.dynamsoft.com/customer/license/trialLicense/?product=dbr)

## Supported Platforms
- **Android**
- **iOS**
- **Windows**
- **Linux**
- **macOS**
- **Web**

## Supported Barcode Symbologies
- Linear Barcodes (1D)
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

- 2D Barcodes
  - QR Code (including Micro QR Code and Model 1)
  - Data Matrix
  - PDF417 (including Micro PDF417)
  - Aztec Code
  - MaxiCode (mode 2-5)
  - DotCode

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

## SDK Version Used for Different Platforms
| Dynamsoft Barcode Reader      | Android |    iOS | Windows | Linux | macOS | Web|
| ----------- | ----------- | ----------- | ----------- |----------- |----------- |----------- |
| Version    | 9.6.20       | 9.6.21   | 9.6.20      | 9.6.20    |9.6.20      | 9.6.20     |

## Build Configuration

### Android
Change the minimum Android sdk version to 21 (or higher) in your `android/app/build.gradle` file.

```
minSdkVersion 21
```

### iOS
Add the keys to `ios/Runner/Info.plist` to make camera work:

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

- Import `DynamsoftBarcodeReader.h` to the bridging header file.
    
  ![macOS bridging header](https://www.dynamsoft.com/codepool/img/2021/flutter/macos-bridging-barcode-header.png)

### Web
In `index.html`, include:

```html
<script src="https://cdn.jsdelivr.net/npm/dynamsoft-javascript-barcode@9.6.20/dist/dbr.js"></script>
```

## API Compatibility
| Methods      | Android |    iOS | Windows | Linux | macOS | Web|
| ----------- | ----------- | ----------- | ----------- |----------- |----------- |----------- |
| `Future<void> setLicense(String license) async`     | :heavy_check_mark:       | :heavy_check_mark:   | :heavy_check_mark:      | :heavy_check_mark:      |:heavy_check_mark:      | :heavy_check_mark:    |
| `Future<List<BarcodeResult>> decodeFile(String filename) async`     | :heavy_check_mark:      | :heavy_check_mark:   | :heavy_check_mark:      |:heavy_check_mark:      | :heavy_check_mark:     |:heavy_check_mark:      |
| `Future<List<BarcodeResult>> decodeImageBuffer(Uint8List bytes, int width, int height, int stride, int format) async`     | :heavy_check_mark:      | :heavy_check_mark:   | :heavy_check_mark:      |:heavy_check_mark:      | :heavy_check_mark:     |:heavy_check_mark:    |
| `Future<int> setBarcodeFormats(int formats) async`     | :heavy_check_mark:       | :heavy_check_mark:   | :heavy_check_mark:       | :heavy_check_mark:       |:heavy_check_mark:      | :heavy_check_mark:     |
| `Future<String> getParameters() async`     | :heavy_check_mark:         | :heavy_check_mark:   | :heavy_check_mark:       | :heavy_check_mark:        |:heavy_check_mark:       | :heavy_check_mark:     |
| `Future<int> setParameters(String params)` async | :heavy_check_mark:         |:heavy_check_mark:   | :heavy_check_mark:       | :heavy_check_mark:        |:heavy_check_mark:      | :heavy_check_mark:     |
| `Future<void> init()` async | :heavy_check_mark:         |:heavy_check_mark:   | :heavy_check_mark:       | :heavy_check_mark:        |:heavy_check_mark:      | :heavy_check_mark:     |

## Usage
- Initialize Flutter barcode SDK and set license key:
    
  ```dart
  _barcodeReader = FlutterBarcodeSdk();
  await _barcodeReader.setLicense('DLS2eyJoYW5kc2hha2VDb2RlIjoiMjAwMDAxLTE2NDk4Mjk3OTI2MzUiLCJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSIsInNlc3Npb25QYXNzd29yZCI6IndTcGR6Vm05WDJrcEQ5YUoifQ==');
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
  await _barcodeReader.setBarcodeFormats(BarcodeFormat.ALL);
  ```

- Get current barcode detection [parameters](https://www.dynamsoft.com/barcode-reader/parameters/reference/image-parameter/?ver=latest):
    
  ```dart
  String params = await _barcodeReader.getParameters();
  // Convert parameters to a JSON object.
  dynamic obj = jsonDecode(params);
  // Modify parameters.
  if (obj['ImageParameter'] != null) {
    obj['ImageParameter']['DeblurLevel'] = 5;
  } else
    obj['deblurLevel'] = 5;
  ```

- Set barcode detection parameters:
    
  ```dart
  int ret = await _barcodeReader.setParameters(json.encode(obj));
  ```


## Try Barcode Decoding Example

### Mobile
The example allows users to scan barcode Qr code via the camera video stream and read barcode QRCode by a static picture.

```
cd example
flutter run -d <device>
```

Video Scan

![flutter barcode scanner](https://www.dynamsoft.com/codepool/img/2021/flutter-barcode-scanner-camera.gif)

Picture Scan

![flutter barcode reader](https://www.dynamsoft.com/codepool/img/2021/flutter-picture-barcode-scan.jpg)


For building Android release app, configure `build.gradle` and corresponding proguard file:

**build.gradle**
```
buildTypes {
        release {
            // TODO: Add your own signing config for the release build.
            // Signing with the debug keys for now, so `flutter run --release` works.
            signingConfig signingConfigs.debug
            proguardFiles getDefaultProguardFile('proguard-android-optimize.txt'), 'proguard-rules.pro'
        }
    }

```

**proguard-rules.pro**

```
-keep class com.dynamsoft.dbr.** { *; }
```

### Windows, Linux and macOS Desktop
Input a valid image path for barcode decoding.

![flutter windows barcode reader](https://www.dynamsoft.com/codepool/img/2021/flutter-desktop-barcode-reader.png)

```
cd example
# Windows
flutter run -d windows

# Linux
flutter run -d linux

# macOS
flutter run -d macos
```

### Web Browser

```
cd example
flutter run -d chrome
```

![flutter web barcode reader](https://www.dynamsoft.com/codepool/img/2021/flutter-web-barcode-sdk.png)








