import 'package:flutter_barcode_sdk/barcode_manager.dart';
import 'package:js/js.dart';

import 'utils.dart';

/// DBRWrapper class.
@JS('DBRWrapper')
class DBRWrapper {
  external PromiseJsImpl<BarcodeScanner> createDefaultScanner(Function func);
  external void patchOverlay();
  external void clearOverlay();
}
