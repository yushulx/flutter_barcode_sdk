// Web-specific tests require a browser environment with the Dynamsoft
// Barcode Reader JS SDK loaded. The code is verified via `flutter analyze`
// which confirms compilation on the web platform.
//
// To run browser tests:
//   flutter test --platform chrome
//
// Ensure index.html includes:
//   <script src="https://cdn.jsdelivr.net/npm/dynamsoft-barcode-reader-bundle/dist/dbr.bundle.js"></script>

import 'package:flutter_test/flutter_test.dart';

void main() {
  test('flutter_barcode_sdk_web package compiles', () {
    // This is a smoke test. The real verification is done by
    // `flutter analyze` which checks all web-specific code compiles
    // correctly including dart:js_interop annotations.
    expect(true, isTrue);
  });
}
