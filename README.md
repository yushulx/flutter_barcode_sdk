# flutter_barcode_sdk
![pub.dev](https://img.shields.io/pub/v/flutter_barcode_sdk.svg)

The **Flutter Barcode SDK** is a wrapper for the [Dynamsoft Barcode Reader SDK](https://www.dynamsoft.com/barcode-reader/overview/). It supports multiple platforms, including **Android**, **iOS**, **Web**, **Windows**, and **Linux**, and can decode a wide range of barcode types such as **linear barcodes**, **QR Code**, **DataMatrix**, **MaxiCode**, **PDF417**, and more. This SDK encapsulates the low-level decoding capabilities of the Dynamsoft Barcode Reader, enabling barcode decoding from both **files and image buffers**. With this package, developers can effortlessly build robust **1D/2D barcode reader** and **scanner** apps. The project is actively maintained with contributions from the community.

> **Note:** For live camera scenarios, it is recommended to use the official [Dynamsoft Capture Vision Flutter Edition](https://pub.dev/packages/dynamsoft_capture_vision_flutter), which offers better performance than combining the [Flutter camera plugin](https://pub.dev/packages/camera) with the Flutter Barcode SDK.

## **Table of Contents**
1. [Getting a License Key](#getting-a-license-key)
2. [Supported Platforms](#supported-platforms)
3. [Supported Barcode Symbologies](#supported-barcode-symbologies)
4. [Build Configuration](#build-configuration)
5. [API Reference](#api-reference)
6. [Examples](#try-barcode-decoding-example)

## **Getting a License Key**
[![](https://img.shields.io/badge/Get-30--day%20FREE%20Trial-blue)](https://www.dynamsoft.com/customer/license/trialLicense/?product=dcv&package=cross-platform)

## **Supported Platforms**
- ✅ **Android**
- ✅ **iOS**
- ✅ **Windows**
- ✅ **Linux**
- ✅ **Web**

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

```yaml
minSdkVersion 21
```

### iOS
Add camera usage descriptions to `ios/Runner/Info.plist`:

```xml
<key>NSCameraUsageDescription</key>
<string>Can I use the camera please?</string>
<key>NSMicrophoneUsageDescription</key>
<string>Can I use the mic please?</string>
```

### Desktop

**Windows & Linux**

Install `CMake` and `platform-specific C++ compiler`.


### Web
In `index.html`, include:

```html
<script src="https://cdn.jsdelivr.net/npm/dynamsoft-capture-vision-bundle@2.6.1000/dist/dcv.bundle.min.js"></script>
```

## API Reference
| Method                                                                                                                        | Description                                                                             | Parameters                                                                                                                                                                                    | Return Type                   |
| ----------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------- |
| `Future<int> setLicense(String license)`                                                                                      | Sets the Dynamsoft Barcode Reader license key.                                          | `license`: The license key string.                                                                                                                                                            | `Future<int>`                 |
| `Future<int> init()`                                                                                                          | Initializes the barcode reader and applies default parameters for the current platform. | —                                                                                                                                                                                             | `Future<int>`                 |
| `Future<List<BarcodeResult>> decodeFile(String filename)`                                                                     | Decodes barcodes from an image file.                                                    | `filename`: Path to the image file.                                                                                                                                                           | `Future<List<BarcodeResult>>` |
| `Future<List<BarcodeResult>> decodeImageBuffer(Uint8List bytes, int width, int height, int stride, int format, int rotation)` | Decodes barcodes from a raw image buffer (for real-time or camera preview scanning).    | `bytes`: Image buffer (raw bytes)<br>`width`: Image width<br>`height`: Image height<br>`stride`: Bytes per row<br>`format`: Pixel format (see `ImagePixelFormat`)<br>`rotation`: 0/90/180/270 | `Future<List<BarcodeResult>>` |
| `Future<int> setBarcodeFormats(int formats)`                                                                                  | Sets the barcode formats to be detected.                                                | `formats`: Bitwise combination of barcode formats (see `BarcodeFormat`).                                                                                                                      | `Future<int>`                 |
| `Future<String> getParameters()`                                                                                              | Retrieves the current barcode detection settings as a JSON string.                      | —                                                                                                                                                                                             | `Future<String>`              |
| `Future<int> setParameters(String params)`                                                                                    | Updates barcode detection parameters with a JSON string.                                | `params`: JSON string of detection parameters.                                                                                                                                                | `Future<int>`                 |



## Try Barcode Reader & Scanner Examples

### Android/iOS
The example demonstrates how to use the Flutter Barcode SDK to read barcodes from an image file and decode the barcode image buffer from the camera stream on Android and iOS.

```
cd example
flutter run 
```

- **Barcode Scanner**

    ![flutter barcode scanner](https://www.dynamsoft.com/codepool/img/2025/01/flutter-android-barcode-scanner.jpg)

- **Barcode Reader**

    ![flutter barcode reader](https://www.dynamsoft.com/codepool/img/2025/01/flutter-android-barcode-image-detection.jpg)


### Windows & Linux 
Run the desktop barcode reader and scanner application on Windows or Linux:

```bash
cd example
# Windows
flutter run -d windows

# Linux
flutter run -d linux
```


### Web Barcode Scanner

```bash
cd example
flutter run -d chrome
```

![flutter web barcode reader](https://www.dynamsoft.com/codepool/img/2025/01/flutter-barcode-scanner-web.png)








