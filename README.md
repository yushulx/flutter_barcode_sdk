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

