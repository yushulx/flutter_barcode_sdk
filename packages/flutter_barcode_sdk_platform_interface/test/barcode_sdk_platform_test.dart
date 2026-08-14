import 'dart:typed_data';

import 'package:flutter_test/flutter_test.dart';
import 'package:flutter_barcode_sdk_platform_interface/flutter_barcode_sdk_platform_interface.dart';

void main() {
  group('BarcodeSdkPlatform', () {
    test('default instance is MethodChannelBarcodeSdk', () {
      expect(BarcodeSdkPlatform.instance, isA<MethodChannelBarcodeSdk>());
    });

    test('set instance rejects non-subclass via token verification', () {
      expect(
        () => BarcodeSdkPlatform.instance = _InvalidImplementation(),
        throwsA(isA<AssertionError>()),
      );
    });

    test('unimplemented methods throw UnimplementedError', () {
      final impl = _StubBarcodeSdkPlatform();

      expect(() => impl.setLicense('key'), throwsUnimplementedError);
      expect(() => impl.init(), throwsUnimplementedError);
      expect(() => impl.decodeFile('path'), throwsUnimplementedError);
      expect(
        () => impl.decodeImageBuffer(
          Uint8List(0), 0, 0, 0, 0, 0,
        ),
        throwsUnimplementedError,
      );
      expect(() => impl.setBarcodeFormats(0), throwsUnimplementedError);
      expect(() => impl.getParameters(), throwsUnimplementedError);
      expect(() => impl.setParameters('{}'), throwsUnimplementedError);
    });
  });

  group('BarcodeFormat', () {
    test('NULL is zero', () {
      expect(BarcodeFormat.NULL, 0x00);
    });

    test('ONED includes common 1D formats', () {
      expect(BarcodeFormat.ONED & BarcodeFormat.CODE_39, BarcodeFormat.CODE_39);
      expect(
        BarcodeFormat.ONED & BarcodeFormat.CODE_128,
        BarcodeFormat.CODE_128,
      );
      expect(BarcodeFormat.ONED & BarcodeFormat.EAN_13, BarcodeFormat.EAN_13);
    });

    test('QR_CODE is a 2D format', () {
      expect(BarcodeFormat.QR_CODE, 0x04000000);
    });

    test('ALL covers QR_CODE', () {
      // ALL is 0xFFFFFFFEFFFFFFFF (unsigned), stored as signed int.
      // Bitwise AND with QR_CODE should yield QR_CODE.
      expect(BarcodeFormat.ALL & BarcodeFormat.QR_CODE, BarcodeFormat.QR_CODE);
    });
  });

  group('ImagePixelFormat', () {
    test('has expected number of formats', () {
      expect(ImagePixelFormat.values.length, 13);
    });

    test('IPF_NV21 exists', () {
      expect(ImagePixelFormat.values, contains(ImagePixelFormat.IPF_NV21));
    });

    test('IPF_RGB_888 exists', () {
      expect(ImagePixelFormat.values, contains(ImagePixelFormat.IPF_RGB_888));
    });
  });

  group('ImageRotation', () {
    test('has four rotation values', () {
      expect(ImageRotation.values.length, 4);
    });

    test('rotation values are correct', () {
      expect(ImageRotation.rotation0.value, 0);
      expect(ImageRotation.rotation90.value, 90);
      expect(ImageRotation.rotation180.value, 180);
      expect(ImageRotation.rotation270.value, 270);
    });
  });
}

/// An invalid implementation that does not extend [BarcodeSdkPlatform].
///
/// Used to verify that [PlatformInterface.verify] rejects it.
class _InvalidImplementation implements BarcodeSdkPlatform {
  @override
  dynamic noSuchMethod(Invocation invocation) => super.noSuchMethod(invocation);
}

/// A minimal stub that extends [BarcodeSdkPlatform] but does not override
/// any methods. Used to verify that the default implementations throw
/// [UnimplementedError].
class _StubBarcodeSdkPlatform extends BarcodeSdkPlatform {}
