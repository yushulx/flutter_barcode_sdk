# flutter_barcode_sdk

[![pub package](https://img.shields.io/pub/v/flutter_barcode_sdk.svg)](https://pub.dev/packages/flutter_barcode_sdk)
[![GitHub license](https://img.shields.io/github/license/yushulx/flutter_barcode_sdk)](https://github.com/yushulx/flutter_barcode_sdk/blob/main/LICENSE)

A cross-platform Flutter plugin for barcode reading and scanning, powered by the [Dynamsoft Barcode Reader SDK](https://www.dynamsoft.com/barcode-reader/overview/). Supports **Android**, **iOS**, **Web**, **Windows**, and **Linux**.

Decode a wide range of 1D and 2D barcode symbologies from image files and raw pixel buffers. Build robust barcode reader and scanner applications with minimal effort.

> **Tip:** For production-grade live camera scanning, consider using the official [Dynamsoft Capture Vision Flutter Edition](https://pub.dev/packages/dynamsoft_capture_vision_flutter) which offers optimized real-time performance.


## Table of Contents

- [Getting Started](#getting-started)
- [Supported Platforms](#supported-platforms)
- [Supported Barcode Symbologies](#supported-barcode-symbologies)
- [Platform Configuration](#platform-configuration)
- [Usage](#usage)
- [API Reference](#api-reference)
- [Examples](#examples)
- [License](#license)


## Getting Started

### 1. Install the Package

```yaml
dependencies:
  flutter_barcode_sdk: ^4.1.0
```

### 2. Obtain a License Key

A valid license is required to activate barcode decoding functionality.

[![Get Trial License](https://img.shields.io/badge/Get-30--day%20FREE%20Trial-blue)](https://www.dynamsoft.com/customer/license/trialLicense/?product=dcv&package=cross-platform)

### 3. Initialize the SDK

```dart
import 'package:flutter_barcode_sdk/flutter_barcode_sdk.dart';
import 'package:flutter_barcode_sdk/dynamsoft_barcode.dart';

final barcodeReader = FlutterBarcodeSdk();
await barcodeReader.setLicense('YOUR-LICENSE-KEY');
await barcodeReader.init();
```

## Supported Platforms

| Platform | Status |
|----------|--------|
| Android  | Supported      |
| iOS      | Supported      |
| Web      | Supported      |
| Windows  | Supported      |
| Linux    | Supported      |


## Supported Barcode Symbologies

### Linear Barcodes (1D)

Code 39 (including Extended) | Code 93 | Code 128 | Code 11 | Code 32 | Codabar | Interleaved 2 of 5 | Industrial 2 of 5 | Matrix 2 of 5 | EAN-8 | EAN-13 | UPC-A | UPC-E | MSI Code | Telepen | Telepen Numeric

### 2D Barcodes

QR Code (including Micro QR) | Data Matrix | PDF417 (including Micro PDF417) | Aztec Code | MaxiCode (modes 2-5) | DotCode

### GS1 DataBar

Omnidirectional | Truncated | Stacked | Stacked Omnidirectional | Expanded | Expanded Stacked | Limited

### Postal Codes

USPS Intelligent Mail | Postnet | Planet | Australian Post | Royal Mail (RM4SCC) | KIX

### Other

Patch Code | GS1 Composite Code | Pharmacode (One-Track / Two-Track) | Non-standard Barcode


## Platform Configuration

### Android

Set the minimum SDK version in `android/app/build.gradle`:

```groovy
minSdkVersion 21
```

### iOS

Add camera usage descriptions to `ios/Runner/Info.plist`:

```xml
<key>NSCameraUsageDescription</key>
<string>Camera access is required for barcode scanning.</string>
<key>NSMicrophoneUsageDescription</key>
<string>Microphone access is required by the camera plugin.</string>
```

### Windows and Linux

Ensure `CMake` and a platform-specific C++ compiler are installed.

### Web

Include the Dynamsoft JavaScript SDK in `web/index.html`:

```html
<script src="https://cdn.jsdelivr.net/npm/dynamsoft-barcode-reader-bundle@11.2.4000/dist/dbr.bundle.js"></script>
```


## Usage

### Decode from an Image File

```dart
List<BarcodeResult> results = await barcodeReader.decodeFile('path/to/image.png');

for (var result in results) {
  print('${result.format}: ${result.text}');
}
```

### Decode from a Camera Buffer

```dart
List<BarcodeResult> results = await barcodeReader.decodeImageBuffer(
  bytes,      // Uint8List - raw pixel data
  width,      // Image width
  height,     // Image height
  stride,     // Bytes per row
  format,     // Pixel format index (e.g., ImagePixelFormat.IPF_NV21.index)
  rotation,   // 0, 90, 180, or 270
);
```

### Set Barcode Formats

```dart
await barcodeReader.setBarcodeFormats(
  BarcodeFormat.QR_CODE | BarcodeFormat.CODE_128 | BarcodeFormat.EAN_13,
);
```

### Configure Advanced Parameters

```dart
String params = await barcodeReader.getParameters();
// Modify the JSON string as needed...
await barcodeReader.setParameters(params);
```

## API Reference

| Method | Description | Return |
|--------|-------------|--------|
| `setLicense(String license)` | Activates the SDK with a license key. | `Future<int>` |
| `init()` | Initializes the barcode reader with default parameters. | `Future<int>` |
| `decodeFile(String filename)` | Decodes barcodes from an image file. | `Future<List<BarcodeResult>>` |
| `decodeImageBuffer(...)` | Decodes barcodes from raw pixel data (camera preview, etc.). | `Future<List<BarcodeResult>>` |
| `setBarcodeFormats(int formats)` | Sets which barcode formats to detect. | `Future<int>` |
| `getParameters()` | Returns the current detection settings as JSON. | `Future<String>` |
| `setParameters(String params)` | Updates detection settings from a JSON string. | `Future<int>` |

See [`BarcodeFormat`](lib/dynamsoft_barcode.dart) for all available format constants and [`ImagePixelFormat`](lib/flutter_barcode_sdk.dart) for supported pixel formats.


## Examples

### Mobile (Android / iOS)

```bash
cd example
flutter run
```

| Barcode Scanner | Barcode Reader |
|:-:|:-:|
| ![Scanner](https://www.dynamsoft.com/codepool/img/2025/01/flutter-android-barcode-scanner.jpg) | ![Reader](https://www.dynamsoft.com/codepool/img/2025/01/flutter-android-barcode-image-detection.jpg) |

### Desktop (Windows / Linux)

```bash
cd example
flutter run -d windows   # or -d linux
```

### Web

```bash
cd example
flutter run -d chrome
```

![Web Scanner](https://www.dynamsoft.com/codepool/img/2025/01/flutter-barcode-scanner-web.png)


