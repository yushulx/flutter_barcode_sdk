# flutter_barcode_sdk
![pub.dev](https://img.shields.io/pub/v/flutter_barcode_sdk.svg)

The Flutter barcode SDK plugin is a wrapper for [Dynamsoft Barcode Reader SDK](https://www.dynamsoft.com/barcode-reader/overview/). It aims to cover Android, iOS, Web, Windows, Linux and macOS, supporting linear barcode, QR Code, DataMatrix, MaxiCode, PDF417, etc.

## License Key for SDK
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
| Version    | 9.0       | 9.0   | 9.0      | 9.0    |9.0      | 9.0     |

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
Include `<script src="https://cdn.jsdelivr.net/npm/dynamsoft-javascript-barcode@9.0.0/dist/dbr.js"></script>` to `index.html`.

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
| `Future<void> decodeVideo(Function callback) async`     | :x:       | :x:   | :x:       | :x:       |:x:       | :heavy_check_mark:     |
| `Future<void> closeVideo() async`     | :x:       | :x:   | :x:       | :x:       |:x:       | :heavy_check_mark:     |

## Usage
- Initialize Flutter barcode SDK and set license key:
    
  ```dart
  _barcodeReader = FlutterBarcodeSdk();
  await _barcodeReader.setLicense('LICENSE-KEY');
  await _barcodeReader.init();
  ```

- Read barcodes from an image file:

  ```dart
  List<BarcodeResult> results = await _barcodeReader.decodeFile(image-path);
  ```

- Read barcodes from video stream [CameraImage](https://pub.dev/documentation/camera/latest/camera/CameraImage-class.html):

  ```dart
  CameraImage availableImage;
  int format = FlutterBarcodeSdk.IF_UNKNOWN;

  switch (availableImage.format.group) {
    case ImageFormatGroup.yuv420:
      format = FlutterBarcodeSdk.IF_YUV420;
      break;
    case ImageFormatGroup.bgra8888:
      format = FlutterBarcodeSdk.IF_BRGA8888;
      break;
    default:
      format = FlutterBarcodeSdk.IF_UNKNOWN;
  }

  List<BarcodeResult> results = _barcodeReader.decodeImageBuffer(
                availableImage.planes[0].bytes,
                availableImage.width,
                availableImage.height,
                availableImage.planes[0].bytesPerRow,
                format);
  ```

- Read barcodes from web browser video stream:

  ```dart
  _barcodeReader.decodeVideo(
                              (results) => {updateResults(results)});
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
The example allows users to scan barcodes via the camera video stream in real-time or read barcodes by taking a picture.

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

**A Better Barcode and QR Code Scanner Plugin**

To get better user experience for scanning barcodes and QR code in real-time, it is recommended to use the [https://pub.dev/packages/flutter_camera_qrcode_scanner](https://pub.dev/packages/flutter_camera_qrcode_scanner) plugin.

### Windows, Linux and macOS Desktop
Input a valid image path for barcode decoding.

![flutter windows barcode reader](https://www.dynamsoft.com/codepool/img/2021/flutter-desktop-barcode-reader.png)

- Windows

  ```
  cd example
  flutter run -d windows
  ```


- Linux 

  ```
  cd example
  flutter run -d linux
  ```

- macOS

  ```
  cd example
  flutter run -d macos
  ```


### Web Browser

```
cd example
flutter run -d chrome
```

Barcode Reader

![flutter web barcode reader](https://www.dynamsoft.com/codepool/img/2021/flutter-web-barcode-sdk.png)

Barcode Scanner

![flutter web barcode scanner](https://www.dynamsoft.com/codepool/img/2021/flutter-web-barcode-scanner.png)






