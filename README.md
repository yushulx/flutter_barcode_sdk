# flutter_barcode_sdk

A flutter plugin project for [Dynamsoft Barcode Reader SDK](https://www.dynamsoft.com/barcode-reader/overview/).

## Try example

```
cd example
flutter run -d <device>
```

![flutter barcode reader](https://www.codepool.biz/wp-content/uploads/2019/08/flutter-barcode-plugin.png)

## Supported Platforms
- Android

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


## Usage

Get a [30-day FREE Trial license](https://www.dynamsoft.com/customer/license/trialLicense/?product=dbr) and set it as follows:

```dart
Future<void> initDynamsoftBarcodeReaderState() async {
    FlutterBarcodeSdk _barcodeReader = FlutterBarcodeSdk();
    await _barcodeReader.initLicense('LICENSE-KEY');
  }
```

Read barcodes from an image file:

```dart
String results = await _barcodeReader.decodeFile(image-path);
```

## License Agreement
https://www.dynamsoft.com/Products/barcode-reader-license-agreement.aspx

## Contact Us
support@dynamsoft.com

## TODO
iOS, Windows, Web
