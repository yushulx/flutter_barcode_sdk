@JS('Dynamsoft')
library dynamsoft;

import 'dart:js';

import 'package:js/js.dart';

import 'utils.dart';

@JS('DBR')
class DBR {
  external static Object get EnumBarcodeFormat;
}

@JS('DBR.BarcodeScanner')
class BarcodeScanner {
  external static PromiseJsImpl<BarcodeScanner> createInstance();
  external void show();
}

@JS('DBR.BarcodeReader')
class BarcodeReader {
  external static PromiseJsImpl<BarcodeReader> createInstance();
}

class BarcodeManager {
  late BarcodeScanner _barcodeScanner;

  void initialize(BarcodeScanner scanner) {
    _barcodeScanner = scanner;
  }

  BarcodeManager() {
    handleThenable(BarcodeScanner.createInstance())
        .then((scanner) => {initialize(scanner)});
  }

  void decodeVideo() {
    _barcodeScanner.show();
  }

  List<Map<dynamic, dynamic>> decodeFile(String file) {
    List<Map<dynamic, dynamic>> results = [];

    // TODO
    var result = Map<String, String>();
    result['test'] = 'test';
    results.add(result);

    return results;
  }
}
