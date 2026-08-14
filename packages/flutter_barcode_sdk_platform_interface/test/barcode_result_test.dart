import 'dart:typed_data';

import 'package:flutter_test/flutter_test.dart';
import 'package:flutter_barcode_sdk_platform_interface/flutter_barcode_sdk_platform_interface.dart';

void main() {
  group('BarcodeResult', () {
    test('fromJson creates a valid BarcodeResult', () {
      final json = <dynamic, dynamic>{
        'format': 'QR_CODE',
        'text': '',
        'x1': 10,
        'y1': 20,
        'x2': 30,
        'y2': 20,
        'x3': 30,
        'y3': 40,
        'x4': 10,
        'y4': 40,
        'angle': 0,
        'barcodeBytes': [72, 101, 108, 108, 111],
        'errorCode': 0,
        'errorMsg': '',
      };

      final result = BarcodeResult.fromJson(json);

      expect(result.format, 'QR_CODE');
      expect(result.text, 'Hello');
      expect(result.x1, 10);
      expect(result.y1, 20);
      expect(result.x2, 30);
      expect(result.y2, 20);
      expect(result.x3, 30);
      expect(result.y3, 40);
      expect(result.x4, 10);
      expect(result.y4, 40);
      expect(result.angle, 0);
      expect(result.barcodeBytes, Uint8List.fromList([72, 101, 108, 108, 111]));
      expect(result.errorCode, 0);
      expect(result.errorMsg, '');
    });

    test('toJson round-trips the data correctly', () {
      final original = BarcodeResult(
        'CODE_128',
        'Test',
        1, 2, 3, 4, 5, 6, 7, 8,
        90,
        Uint8List.fromList([84, 101, 115, 116]),
        0,
        '',
      );

      final json = original.toJson();
      final restored = BarcodeResult.fromJson(json);

      expect(restored.format, original.format);
      expect(restored.text, original.text);
      expect(restored.x1, original.x1);
      expect(restored.y1, original.y1);
      expect(restored.x2, original.x2);
      expect(restored.y2, original.y2);
      expect(restored.x3, original.x3);
      expect(restored.y3, original.y3);
      expect(restored.x4, original.x4);
      expect(restored.y4, original.y4);
      expect(restored.angle, original.angle);
      expect(restored.barcodeBytes, original.barcodeBytes);
      expect(restored.errorCode, original.errorCode);
      expect(restored.errorMsg, original.errorMsg);
    });
  });

  group('convertResults', () {
    test('converts a list of JSON maps to BarcodeResult list', () {
      final jsonResults = <Map<dynamic, dynamic>>[
        <dynamic, dynamic>{
          'format': 'QR_CODE',
          'text': '',
          'x1': 0, 'y1': 0, 'x2': 0, 'y2': 0,
          'x3': 0, 'y3': 0, 'x4': 0, 'y4': 0,
          'angle': 0,
          'barcodeBytes': [65],
          'errorCode': 0,
          'errorMsg': '',
        },
        <dynamic, dynamic>{
          'format': 'CODE_128',
          'text': '',
          'x1': 1, 'y1': 1, 'x2': 2, 'y2': 2,
          'x3': 3, 'y3': 3, 'x4': 4, 'y4': 4,
          'angle': 45,
          'barcodeBytes': [66],
          'errorCode': 0,
          'errorMsg': '',
        },
      ];

      final results = convertResults(jsonResults);

      expect(results.length, 2);
      expect(results[0].format, 'QR_CODE');
      expect(results[0].text, 'A');
      expect(results[1].format, 'CODE_128');
      expect(results[1].text, 'B');
    });

    test('returns empty list for empty input', () {
      expect(convertResults([]), isEmpty);
    });
  });
}
