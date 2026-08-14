import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:flutter_barcode_sdk_platform_interface/flutter_barcode_sdk_platform_interface.dart';
import 'package:flutter_barcode_sdk/flutter_barcode_sdk.dart';

void main() {
  const MethodChannel channel = MethodChannel('flutter_barcode_sdk');
  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    TestDefaultBinaryMessengerBinding
        .instance.defaultBinaryMessenger
        .setMockMethodCallHandler(channel, (MethodCall methodCall) async {
      switch (methodCall.method) {
        case 'setLicense':
          return 0;
        case 'init':
          return 0;
        case 'setParameters':
          return 0;
        case 'getParameters':
          return '{}';
        case 'setBarcodeFormats':
          return 0;
        case 'decodeFile':
          return <Map<dynamic, dynamic>>[
            <dynamic, dynamic>{
              'format': 'QR_CODE',
              'text': '',
              'x1': 0, 'y1': 0, 'x2': 0, 'y2': 0,
              'x3': 0, 'y3': 0, 'x4': 0, 'y4': 0,
              'angle': 0,
              'barcodeBytes': [72, 105],
              'errorCode': 0,
              'errorMsg': '',
            },
          ];
        case 'decodeImageBuffer':
          return <Map<dynamic, dynamic>>[];
        default:
          return null;
      }
    });
  });

  tearDown(() {
    TestDefaultBinaryMessengerBinding
        .instance.defaultBinaryMessenger
        .setMockMethodCallHandler(channel, null);
  });

  group('FlutterBarcodeSdk', () {
    test('can be instantiated', () {
      expect(FlutterBarcodeSdk(), isNotNull);
    });

    test('delegates to BarcodeSdkPlatform.instance', () {
      expect(BarcodeSdkPlatform.instance, isA<MethodChannelBarcodeSdk>());
    });

    test('setLicense calls platform channel', () async {
      final reader = FlutterBarcodeSdk();
      final result = await reader.setLicense('test-license');
      expect(result, 0);
    });

    test('init calls platform channel then setParameters', () async {
      final reader = FlutterBarcodeSdk();
      final result = await reader.init();
      expect(result, 0);
    });

    test('decodeFile returns BarcodeResult list', () async {
      final reader = FlutterBarcodeSdk();
      final results = await reader.decodeFile('test.png');
      expect(results.length, 1);
      expect(results[0].format, 'QR_CODE');
      expect(results[0].text, 'Hi');
    });

    test('decodeImageBuffer returns BarcodeResult list', () async {
      final reader = FlutterBarcodeSdk();
      final results = await reader.decodeImageBuffer(
          Uint8List(0), 0, 0, 0, 0, 0);
      expect(results, isEmpty);
    });

    test('setBarcodeFormats calls platform channel', () async {
      final reader = FlutterBarcodeSdk();
      final result = await reader.setBarcodeFormats(BarcodeFormat.QR_CODE);
      expect(result, 0);
    });

    test('getParameters calls platform channel', () async {
      final reader = FlutterBarcodeSdk();
      final result = await reader.getParameters();
      expect(result, '{}');
    });

    test('setParameters calls platform channel', () async {
      final reader = FlutterBarcodeSdk();
      final result = await reader.setParameters('{}');
      expect(result, 0);
    });
  });

  group('Re-exports from platform interface', () {
    test('BarcodeResult is accessible', () {
      final result = BarcodeResult(
        'CODE_128', 'test', 0, 0, 0, 0, 0, 0, 0, 0, 0,
        Uint8List(0), 0, '',
      );
      expect(result.format, 'CODE_128');
    });

    test('BarcodeFormat is accessible', () {
      expect(BarcodeFormat.QR_CODE, 0x04000000);
    });

    test('ImagePixelFormat is accessible', () {
      expect(ImagePixelFormat.IPF_RGB_888, isNotNull);
    });

    test('ImageRotation is accessible', () {
      expect(ImageRotation.rotation90.value, 90);
    });

    test('convertResults is accessible', () {
      final results = convertResults([]);
      expect(results, isEmpty);
    });
  });
}
